/***********************************************************************************
 *                         This file is part of dynareadout
 *                    https://github.com/PucklaJ/dynareadout
 ***********************************************************************************
 * Copyright (c) 2022 Jonas Pucher
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product,
 * an acknowledgment in the product documentation would be appreciated but is
 * not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ************************************************************************************/

#include "key.h"
#include "binary_search.h"
#include "extra_string.h"
#include "profiling.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_COMMENT '$'
#define LINE_WIDTH 80

#define ERROR_ERRNO(msg)                                                       \
  *error_string = malloc(1024);                                                \
  sprintf(*error_string, msg, strerror(errno));

typedef struct {
  keyword_t *current_keyword;
  keyword_t *keywords;
  size_t *num_keywords;
} key_file_parse_data;

void key_file_parse_callback(const char *keyword_name, const card_t *card,
                             size_t card_index, void *user_data) {
  key_file_parse_data *data = (key_file_parse_data *)user_data;

  if (!data->current_keyword || card_index == 0) {
    size_t index = 0;

    if (data->keywords) {
      int found;
      index = key_file_binary_search_insert(
          data->keywords, 0, *data->num_keywords - 1, keyword_name, &found);

      if (found) {
        /* Look for the last occurrence of the keyword*/
        index++;
        if (index != *data->num_keywords) {
          while (strcmp(data->keywords[index].name, keyword_name) == 0) {
            index++;
          }
        }
      }
    }

    (*data->num_keywords)++;
    data->keywords =
        realloc(data->keywords, *data->num_keywords * sizeof(keyword_t));

    /* Move everything to the right*/
    size_t j = *data->num_keywords - 1;
    while (j > index) {
      data->keywords[j] = data->keywords[j - 1];
      j--;
    }

    data->current_keyword = &data->keywords[index];

    data->current_keyword->cards = NULL;
    data->current_keyword->num_cards = 0;

    /* Always allocate LINE_WIDTH bytes*/
    data->current_keyword->name = malloc(LINE_WIDTH);
    strcpy(data->current_keyword->name, keyword_name);
  }

  if (!card) {
    return;
  }

  /* Add the card to the keyword*/
  data->current_keyword->num_cards++;
  data->current_keyword->cards =
      realloc(data->current_keyword->cards,
              data->current_keyword->num_cards * sizeof(card_t));
  card_t *keyword_card =
      &data->current_keyword->cards[data->current_keyword->num_cards - 1];
  keyword_card->string = malloc(LINE_WIDTH + 1);
  memcpy(keyword_card->string, card->string, LINE_WIDTH + 1);
}

keyword_t *key_file_parse(const char *file_name, size_t *num_keywords,
                          int parse_includes, char **error_string) {
  BEGIN_PROFILE_FUNC();

  key_file_parse_data data;
  data.current_keyword = NULL;
  data.keywords = NULL;
  data.num_keywords = num_keywords;
  *num_keywords = 0;

  key_file_parse_with_callback(file_name, key_file_parse_callback,
                               parse_includes, error_string, &data, NULL, NULL);

  END_PROFILE_FUNC();
  return data.keywords;
}

void key_file_parse_with_callback(const char *file_name,
                                  key_file_callback callback,
                                  int parse_includes, char **error_string,
                                  void *user_data, char ***include_paths,
                                  size_t *num_include_paths) {
  BEGIN_PROFILE_FUNC();

  *error_string = NULL;

  FILE *file = fopen(file_name, "rb");
  if (!file) {
    ERROR_ERRNO("Failed to open key file: %s")
    END_PROFILE_FUNC();
    return;
  }

  /* This points to the include paths*/
  char ***include_paths_ptr;
  size_t *num_include_paths_ptr;

  if (!include_paths) {
    include_paths_ptr = malloc(sizeof(char **));
    *include_paths_ptr = NULL;
  } else {
    include_paths_ptr = include_paths;
  }

  if (!num_include_paths) {
    num_include_paths_ptr = malloc(sizeof(size_t));
    *num_include_paths_ptr = 0;
  } else {
    num_include_paths_ptr = num_include_paths;
  }

  /* Add the current directory to the include paths*/
  char *current_wd = path_working_directory();
  if (!current_wd) {
    current_wd = malloc(2);
    current_wd[0] = '.';
    current_wd[1] = '\0';
  }

  (*num_include_paths_ptr)++;
  *include_paths_ptr =
      realloc(*include_paths_ptr, *num_include_paths_ptr * sizeof(char *));
  (*include_paths_ptr)[*num_include_paths_ptr - 1] = current_wd;

  /* Store the parent path of the file if need by INCLUDE_PATH_RELATIVE*/
  char *file_parent_path = NULL;

  extra_string line;
  extra_string current_keyword_name;
  current_keyword_name.buffer[0] = '\0';
  current_keyword_name.extra = NULL;
  line.extra = NULL;

  size_t current_keyword_length = 0;
  size_t card_index = 0;

  while (1) {
    /* Clear extra without freeing the memory*/
    if (line.extra) {
      line.extra[0] = '\0';
    }

    size_t line_length = 0;

    /* Find the new line*/
    /* Read more data from the file until a new line has been found*/
    size_t i = 0;
    size_t comment_index = (size_t)~0;
    /* Read the file in LINE_WIDTH sized chunks, but read LINE_WIDTH + 1
     * characters for the first read, because a lot of lines will be exactly
     * LINE_WIDTH characters long, therefore we read the new line directly in
     * the first read chunk*/
    int n = fread(line.buffer, 1, EXTRA_STRING_BUFFER_SIZE, file);
    if (n == 0 && feof(file)) {
      break;
    } else if (n < EXTRA_STRING_BUFFER_SIZE) {
      line.buffer[n] = '\0';
    }

    while (1) {
      size_t j = 0;
      while (j < (size_t)n) {
        const char c = extra_string_get(&line, i);
        if (c == '\n') {
          break;
        }
        if (c == KEY_COMMENT && comment_index == (size_t)~0) {
          comment_index = i;
        }

        i++;
        j++;
      }

      line_length = i;

      if (j == (size_t)n) {
        /* New line is exactly after the line (perfect!)*/
        if ((i < EXTRA_STRING_BUFFER_SIZE || line.extra != NULL) &&
            extra_string_get(&line, i) == '\n') {
          extra_string_set(&line, i, '\0');
          break;
        }

        /* We still need to read more to find the new line*/
      } else if (j != 0) {
        /* New line is somewhere within LINE_WIDTH (or EXTRA_STRING_BUFFER_SIZE)
         * characters*/
        /* Seek back to the beginning of the line that has been "accidentally"
         * read*/
        fseek(file, j - n + 1, SEEK_CUR);
        extra_string_set(&line, i, '\0');
        break;
      } else {
        /* The first character that we read is a new line. Most of the time this
         * means that
         * 1. The line is exactly EXTRA_STRING_BUFFER_SIZE characters long or
         * 2. The line is empty*/
        fseek(file, 1 - n, SEEK_CUR);
        extra_string_set(&line, i, '\0');
        break;
      }

      /* The file ends without a new line*/
      if (n < EXTRA_STRING_BUFFER_SIZE) {
        break;
      }

      line.extra = realloc(line.extra, i * sizeof(char));
      n = fread(&line.extra[i - EXTRA_STRING_BUFFER_SIZE], 1,
                EXTRA_STRING_BUFFER_SIZE, file);
      if (n == 0 && feof(file)) {
        line.extra[i - EXTRA_STRING_BUFFER_SIZE] = '\0';
        break;
      }
    }

    if (line_length == 0 && n == 0 && feof(file)) {
      break;
    }

    /* Check if the line starts with a comment or contains a comment character*/
    if (comment_index == 0) {
      /* The entire line is a comment. Ignore it.*/
      continue;
    } else if (comment_index != (size_t)~0) {
      extra_string_set(&line, comment_index, '\0');
    }

    /* ------- 🐉 Here be parsings 🐉 --------- */

    /* Check if the line is a keyword (starts with '*')
     * Support lines being preceded by ' ' */
    int is_keyword = 0;
    if (line_length != 0) {
      i = 0;
      while (extra_string_get(&line, i) == ' ') {
        i++;
      }
      is_keyword = extra_string_get(&line, i) == '*';
    }

    if (is_keyword) {
      extra_string_copy(&current_keyword_name, &line, line_length, i + 1);

      /* Compute the length of the keyword*/
      current_keyword_length = 0;
      while (1) {
        const char c =
            extra_string_get(&current_keyword_name, current_keyword_length);
        if (c == ' ' || c == '\0') {
          break;
        }
        current_keyword_length++;
      }
      extra_string_set(&current_keyword_name, current_keyword_length, '\0');

      /* Quit on "END"*/
      if (current_keyword_length == 3 &&
          extra_string_compare(&current_keyword_name, "END") == 0) {
        break;
      }

      card_index = 0;
    } else {
      /* This means that we have a card*/
      card_t card;
      if (line_length < EXTRA_STRING_BUFFER_SIZE) {
        card.string = line.buffer;
      } else {
        card.string = malloc(line_length + 1);
        extra_string_copy_to_string(card.string, &line, line_length);
        card.string[line_length] = '\0';
      }

      if (parse_includes &&
          extra_string_starts_with(&current_keyword_name, "INCLUDE")) {
        /* Parse all the different INCLUDE keywords*/
        if (extra_string_compare(&current_keyword_name, "INCLUDE") == 0) {
          /* TODO: Support multi line file names (Remark 3)*/
          char *include_file_name = card_parse_whole(&card);
          char *final_include_file_name = NULL;

          /* Loop over all include paths and look for file*/
          i = 0;
          while (i < *num_include_paths_ptr) {
            char *full_include_file_name =
                path_join((*include_paths_ptr)[i], include_file_name);

            if (path_is_file(full_include_file_name)) {
              final_include_file_name = full_include_file_name;
              break;
            }

            free(full_include_file_name);

            i++;
          }

          if (final_include_file_name) {
            /* Call the function recursively*/
            key_file_parse_with_callback(
                final_include_file_name, callback, 1, error_string, user_data,
                include_paths_ptr, num_include_paths_ptr);
            free(final_include_file_name);
          } else {
            fprintf(stderr,
                    "The include file \"%s\" could not be found in any of the "
                    "given paths\n",
                    include_file_name);
          }
          free(include_file_name);

          /* continue without calling the callback for the card*/
          if (card.string != line.buffer) {
            free(card.string);
          }

          /* Quit if an error in the recursive call happened*/
          if (*error_string != NULL) {
            break;
          }

          card_index++;
          continue;
        } else if (extra_string_compare(&current_keyword_name,
                                        "INCLUDE_PATH") == 0) {
          /* TODO: Support multi line path names (Remark 3)*/
          char *include_path_name = card_parse_whole(&card);

          (*num_include_paths_ptr)++;
          *include_paths_ptr = realloc(*include_paths_ptr,
                                       *num_include_paths_ptr * sizeof(char *));
          (*include_paths_ptr)[*num_include_paths_ptr - 1] = include_path_name;
        } else if (extra_string_compare(&current_keyword_name,
                                        "INCLUDE_PATH_RELATIVE") == 0) {
          /* TODO: Support multi line path names (Remark 3)*/
          char *include_path_name = card_parse_whole(&card);

          if (!file_parent_path) {
            /* TODO: The relative path probably needs to be relative to the
             * FIRST input given to LS Dyna by the command line
             * 'i=parent_folder/file_name.k'*/
            const size_t index = path_move_up_real(file_name);
            if (index == (size_t)~0) {
              const size_t current_wd_len = strlen(current_wd);
              file_parent_path = malloc(current_wd_len + 1);
              memcpy(file_parent_path, current_wd, current_wd_len + 1);
            } else {
              if (path_is_abs(file_name)) {
                file_parent_path = malloc(index + 1 + 1);
                memcpy(file_parent_path, file_name, index + 1);
                file_parent_path[index + 1] = '\0';
              } else {
                file_parent_path = path_join(current_wd, file_name);
                file_parent_path[path_move_up_real(file_parent_path) + 1] =
                    '\0';
              }
            }
          }

          char *full_include_path_name =
              path_join(file_parent_path, include_path_name);
          free(include_path_name);

          (*num_include_paths_ptr)++;
          *include_paths_ptr = realloc(*include_paths_ptr,
                                       *num_include_paths_ptr * sizeof(char *));
          (*include_paths_ptr)[*num_include_paths_ptr - 1] =
              full_include_path_name;
        } else if (extra_string_compare(&current_keyword_name,
                                        "INCLUDE_BINARY") == 0) {
          /* TODO*/
          fprintf(stderr, "The INCLUDE_BINARY keyword is not implemented\n");
        } else if (extra_string_compare(&current_keyword_name,
                                        "INCLUDE_NASTRAN") == 0) {
          /* TODO*/
          fprintf(stderr, "The INCLUDE_NASTRAN keyword is not implemented\n");
        } else if (extra_string_compare(&current_keyword_name,
                                        "INCLUDE_STAMPED_SET") == 0) {
          /* TODO*/
          fprintf(stderr,
                  "The INCLUDE_STAMPED_SET keyword is not implemented\n");
        } else if (extra_string_starts_with(&current_keyword_name,
                                            "INCLUDE_TRANSFORM") != 0) {
          /* TODO*/
          fprintf(stderr, "The %s keyword is not implemented\n",
                  current_keyword_name.buffer);
        } else if (extra_string_starts_with(&current_keyword_name,
                                            "INCLUDE_STAMPED_PART") != 0) {
          /* TODO*/
          fprintf(stderr, "The %s keyword is not implemented\n",
                  current_keyword_name.buffer);
        }
      }

      char *keyword_name;
      if (current_keyword_length < EXTRA_STRING_BUFFER_SIZE) {
        keyword_name = current_keyword_name.buffer;
      } else {
        keyword_name = malloc(current_keyword_length + 1);
        extra_string_copy_to_string(keyword_name, &current_keyword_name,
                                    current_keyword_length);
        keyword_name[current_keyword_length] = '\0';
      }

      callback(keyword_name, &card, card_index, user_data);

      if (card.string != line.buffer) {
        free(card.string);
      }
      if (keyword_name != current_keyword_name.buffer) {
        free(keyword_name);
      }

      card_index++;
    }

    /* ---------------------------------------- */
  }

  /* Call the callback for the last keyword if it is not "END", because some
   * keywords can have no cards*/
  if (current_keyword_length != 3 ||
      extra_string_compare(&current_keyword_name, "END") != 0) {
    char *keyword_name;
    if (current_keyword_length < EXTRA_STRING_BUFFER_SIZE) {
      keyword_name = current_keyword_name.buffer;
    } else {
      keyword_name = malloc(current_keyword_length + 1);
      extra_string_copy_to_string(keyword_name, &current_keyword_name,
                                  current_keyword_length);
      keyword_name[current_keyword_length] = '\0';
    }

    callback(keyword_name, NULL, (size_t)~0, user_data);

    if (keyword_name != current_keyword_name.buffer) {
      free(keyword_name);
    }
  }

  /* Free all include paths*/
  if (!include_paths) {
    size_t i = 0;
    while (i < *num_include_paths_ptr) {
      free((*include_paths_ptr)[i]);

      i++;
    }
    free(*include_paths_ptr);
    free(include_paths_ptr);
  }
  if (!num_include_paths) {
    free(num_include_paths_ptr);
  }

  free(file_parent_path);
  free(line.extra);
  free(current_keyword_name.extra);

  fclose(file);

  END_PROFILE_FUNC();
}

void key_file_free(keyword_t *keywords, size_t num_keywords) {
  BEGIN_PROFILE_FUNC();

  size_t i = 0;
  while (i < num_keywords) {
    free(keywords[i].name);
    size_t j = 0;
    while (j < keywords[i].num_cards) {
      free(keywords[i].cards[j].string);

      j++;
    }
    free(keywords[i].cards);

    i++;
  }

  free(keywords);

  END_PROFILE_FUNC();
}

keyword_t *key_file_get(keyword_t *keywords, size_t num_keywords,
                        const char *name, size_t index) {
  BEGIN_PROFILE_FUNC();

  if (num_keywords == 0) {
    END_PROFILE_FUNC();
    return NULL;
  }

  const size_t find_index =
      key_file_binary_search(keywords, 0, num_keywords - 1, name);
  if (find_index == (size_t)~0) {
    END_PROFILE_FUNC();
    return NULL;
  }

  if (find_index == 0 && index == 0) {
    END_PROFILE_FUNC();
    return keywords;
  }

  /* Find the first of the keyword*/
  size_t i = find_index;
  while (i > 0 && strcmp(keywords[i].name, name) == 0) {
    i--;
  }

  if (i != 0 || strcmp(keywords[i].name, name) != 0) {
    i++;
  }

  size_t j = 0;
  while (j < index && i < num_keywords) {
    j++;
    i++;
  }

  if (j == index && strcmp(keywords[i].name, name) == 0) {
    keyword_t *value = &keywords[i];
    END_PROFILE_FUNC();
    return value;
  }

  END_PROFILE_FUNC();
  return NULL;
}

void card_parse_begin(card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  card->current_index = 0;
  card->value_width = value_width;

  END_PROFILE_FUNC();
}

void card_parse_next(card_t *card) {
  BEGIN_PROFILE_FUNC();

  card->current_index += card->value_width;

  END_PROFILE_FUNC();
}

void card_parse_next_width(card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  card->current_index += value_width;

  END_PROFILE_FUNC();
}

int card_parse_done(const card_t *card) {
  BEGIN_PROFILE_FUNC();

  const int is_done = card->string[card->current_index] == '\0';

  END_PROFILE_FUNC();
  return is_done;
}

int64_t card_parse_int(const card_t *card) {
  return card_parse_int_width(card, card->value_width);
}

int64_t card_parse_int_width(const card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  errno = 0;

  uint8_t i = card->current_index;
  /* Loop until leading whitespace is trimmed*/
  while (i < card->current_index + value_width && card->string[i] == ' ') {
    i++;
  }

  /* The string is completely empty or just spaces*/
  if (i == card->current_index + value_width || card->string[i] == '\0') {
    errno = EINVAL;

    END_PROFILE_FUNC();
    return 0;
  }

  /* Check for negative sign*/
  int64_t sign = 1;
  if (card->string[i] == '-') {
    sign = -1;
    i++;
  }
  int64_t result = 0;

  /* Iterate over the digits of the string and compute the integer value*/
  while (i < card->current_index + value_width && card->string[i] != '\0') {
    if (card->string[i] >= '0' && card->string[i] <= '9') {
      result = result * 10 + (card->string[i] - '0');
    } else if (card->string[i] == ' ') {
      /* Quit when encountering whitespace*/
      result *= sign;

      END_PROFILE_FUNC();
      return result;
    } else {
      /* If a non-digit character is encountered, return 0 to indicate an
       * error*/
      errno = EINVAL;

      END_PROFILE_FUNC();
      return 0;
    }

    i++;
  }

  result *= sign;

  END_PROFILE_FUNC();
  return result;
}

float card_parse_float32(const card_t *card) {
  return card_parse_float32_width(card, card->value_width);
}

float card_parse_float32_width(const card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  errno = 0;

  float result = 0.0f;
  float fraction = 0.0f;
  float exponent = 1.0f;
  float sign = 1.0f;
  int has_integer = 0;
  int has_fraction = 0;
  int has_exponent = 0;
  int exponent_sign = 1;
  int exponent_value = 0;

  uint8_t i = card->current_index;
  /* Loop until leading whitespace is trimmed*/
  while (i < card->current_index + value_width && card->string[i] == ' ') {
    i++;
  }

  /* The string is completely empty or just spaces*/
  if (i == card->current_index + value_width || card->string[i] == '\0') {
    errno = EINVAL;

    END_PROFILE_FUNC();
    return 0.0f;
  }

  /* Check for sign*/
  if (card->string[i] == '-') {
    sign = -1.0f;
    i++;
  } else if (card->string[i] == '+') {
    i++;
  }

  if (i == card->current_index + value_width || card->string[i] == '\0' ||
      card->string[i] == ' ') {
    errno = EINVAL;

    END_PROFILE_FUNC();
    return 0.0f;
  }

  /* Parse integer part*/
  while (i < card->current_index + value_width && card->string[i] != '\0' &&
         card->string[i] != '.' && card->string[i] != 'e' &&
         card->string[i] != 'E') {
    if (card->string[i] >= '0' && card->string[i] <= '9') {
      has_integer = 1;

      result = result * 10.0f + (card->string[i] - '0');
    } else if (card->string[i] == ' ') {
      /* Quit when encountering whitespace*/
      result *= sign;

      END_PROFILE_FUNC();
      return result;
    } else {
      /* Invalid character*/
      errno = EINVAL;

      END_PROFILE_FUNC();
      return 0.0f;
    }
    i++;
  }

  /* If we already reached the end*/
  if (i == card->current_index + value_width || card->string[i] == '\0') {
    result *= sign;

    END_PROFILE_FUNC();
    return result;
  }

  /* Parse fraction part*/
  if (card->string[i] == '.') {
    i++;
    has_fraction = 1;
    while (i < card->current_index + value_width && card->string[i] != '\0' &&
           card->string[i] != 'e' && card->string[i] != 'E') {
      if (card->string[i] >= '0' && card->string[i] <= '9') {
        fraction = fraction * 10.0f + (card->string[i] - '0');
        exponent *= 10.0f;
        i++;
      } else if (card->string[i] == ' ') {
        /* Quit when encountering whitespace*/
        fraction /= exponent;
        result += fraction;
        result *= sign;

        END_PROFILE_FUNC();
        return result;
      } else {
        /* Invalid character*/
        errno = EINVAL;

        return 0.0;
      }
    }

    /* If we already reached the end*/
    if (i == card->current_index + value_width || card->string[i] == '\0') {
      fraction /= exponent;
      result += fraction;
      result *= sign;

      END_PROFILE_FUNC();
      return result;
    }
  }

  /* Parse exponent part*/
  if (card->string[i] == 'e' || card->string[i] == 'E') {
    if (!has_integer) {
      errno = EINVAL;

      END_PROFILE_FUNC();
      return 0.0f;
    }

    i++;
    if (card->string[i] == '-') {
      exponent_sign = -1;
      i++;
    } else if (card->string[i] == '+') {
      i++;
    }

    if (i == card->current_index + value_width || card->string[i] == '\0' ||
        card->string[i] == ' ') {
      if (has_fraction) {
        fraction /= exponent;
        result += fraction;
      }

      result *= sign;

      errno = EINVAL;

      END_PROFILE_FUNC();
      return result;
    }

    while (i < card->current_index + value_width && card->string[i] != '\0') {
      if (card->string[i] >= '0' && card->string[i] <= '9') {
        has_exponent = 1;

        exponent_value = exponent_value * 10 + (card->string[i] - '0');
        i++;
      } else if (card->string[i] == ' ') {
        /* Quit when encountering whitespace*/
        if (has_fraction) {
          fraction /= exponent;
          result += fraction;
        }
        result *= pow(10, exponent_sign * exponent_value);
        result *= sign;

        END_PROFILE_FUNC();
        return result;
      } else {
        /* Invalid character*/
        errno = EINVAL;

        END_PROFILE_FUNC();
        return 0.0f;
      }
    }

    if (!has_exponent) {
      if (has_fraction) {
        fraction /= exponent;
        result += fraction;
      }

      result *= sign;

      errno = EINVAL;

      END_PROFILE_FUNC();
      return result;
    }
  }

  /* Combine integer, fraction, and exponent parts */
  if (has_fraction) {
    fraction /= exponent;
    result += fraction;
  }
  if (has_exponent) {
    result *= pow(10, exponent_sign * exponent_value);
  }

  result *= sign;

  END_PROFILE_FUNC();
  return result;
}

double card_parse_float64(const card_t *card) {
  return card_parse_float64_width(card, card->value_width);
}

double card_parse_float64_width(const card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  errno = 0;

  double result = 0.0;
  double fraction = 0.0;
  double exponent = 1.0;
  double sign = 1.0;
  int has_integer = 0;
  int has_fraction = 0;
  int has_exponent = 0;
  int exponent_sign = 1;
  int exponent_value = 0;

  uint8_t i = card->current_index;
  /* Loop until leading whitespace is trimmed*/
  while (i < card->current_index + value_width && card->string[i] == ' ') {
    i++;
  }

  /* The string is completely empty or just spaces*/
  if (i == card->current_index + value_width || card->string[i] == '\0') {
    errno = EINVAL;

    END_PROFILE_FUNC();
    return 0.0;
  }

  /* Check for sign*/
  if (card->string[i] == '-') {
    sign = -1.0;
    i++;
  } else if (card->string[i] == '+') {
    i++;
  }

  if (i == card->current_index + value_width || card->string[i] == '\0' ||
      card->string[i] == ' ') {
    errno = EINVAL;

    END_PROFILE_FUNC();
    return 0.0;
  }

  /* Parse integer part*/
  while (i < card->current_index + value_width && card->string[i] != '\0' &&
         card->string[i] != '.' && card->string[i] != 'e' &&
         card->string[i] != 'E') {
    if (card->string[i] >= '0' && card->string[i] <= '9') {
      has_integer = 1;

      result = result * 10.0 + (card->string[i] - '0');
    } else if (card->string[i] == ' ') {
      /* Quit when encountering whitespace*/
      result *= sign;

      END_PROFILE_FUNC();
      return result;
    } else {
      /* Invalid character*/
      errno = EINVAL;

      END_PROFILE_FUNC();
      return 0.0;
    }
    i++;
  }

  /* If we already reached the end*/
  if (i == card->current_index + value_width || card->string[i] == '\0') {
    result *= sign;

    END_PROFILE_FUNC();
    return result;
  }

  /* Parse fraction part*/
  if (card->string[i] == '.') {
    i++;
    has_fraction = 1;
    while (i < card->current_index + value_width && card->string[i] != '\0' &&
           card->string[i] != 'e' && card->string[i] != 'E') {
      if (card->string[i] >= '0' && card->string[i] <= '9') {
        fraction = fraction * 10.0 + (card->string[i] - '0');
        exponent *= 10.0;
        i++;
      } else if (card->string[i] == ' ') {
        /* Quit when encountering whitespace*/
        fraction /= exponent;
        result += fraction;
        result *= sign;

        END_PROFILE_FUNC();
        return result;
      } else {
        /* Invalid character*/
        errno = EINVAL;

        return 0.0;
      }
    }

    /* If we already reached the end*/
    if (i == card->current_index + value_width || card->string[i] == '\0') {
      fraction /= exponent;
      result += fraction;
      result *= sign;

      END_PROFILE_FUNC();
      return result;
    }
  }

  /* Parse exponent part*/
  if (card->string[i] == 'e' || card->string[i] == 'E') {
    if (!has_integer) {
      errno = EINVAL;

      END_PROFILE_FUNC();
      return 0.0;
    }

    i++;
    if (card->string[i] == '-') {
      exponent_sign = -1;
      i++;
    } else if (card->string[i] == '+') {
      i++;
    }

    if (i == card->current_index + value_width || card->string[i] == '\0' ||
        card->string[i] == ' ') {
      if (has_fraction) {
        fraction /= exponent;
        result += fraction;
      }

      result *= sign;

      errno = EINVAL;

      END_PROFILE_FUNC();
      return result;
    }

    while (i < card->current_index + value_width && card->string[i] != '\0') {
      if (card->string[i] >= '0' && card->string[i] <= '9') {
        has_exponent = 1;

        exponent_value = exponent_value * 10 + (card->string[i] - '0');
        i++;
      } else if (card->string[i] == ' ') {
        /* Quit when encountering whitespace*/
        if (has_fraction) {
          fraction /= exponent;
          result += fraction;
        }
        result *= pow(10, exponent_sign * exponent_value);
        result *= sign;

        END_PROFILE_FUNC();
        return result;
      } else {
        /* Invalid character*/
        errno = EINVAL;

        END_PROFILE_FUNC();
        return 0.0;
      }
    }

    if (!has_exponent) {
      if (has_fraction) {
        fraction /= exponent;
        result += fraction;
      }

      result *= sign;

      errno = EINVAL;

      END_PROFILE_FUNC();
      return result;
    }
  }

  /* Combine integer, fraction, and exponent parts */
  if (has_fraction) {
    fraction /= exponent;
    result += fraction;
  }
  if (has_exponent) {
    result *= pow(10, exponent_sign * exponent_value);
  }

  result *= sign;

  END_PROFILE_FUNC();
  return result;
}

char *card_parse_string(const card_t *card) {
  return card_parse_string_width(card, card->value_width);
}

char *card_parse_string_no_trim(const card_t *card) {
  return card_parse_string_width_no_trim(card, card->value_width);
}

char *card_parse_string_width(const card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  uint8_t i = 0;
  while (i < value_width && card->string[i + card->current_index] != '\0' &&
         card->string[i + card->current_index] == ' ') {
    i++;
  }

  /* There is no string. Just spaces*/
  if (i == value_width || card->string[i + card->current_index] == '\0') {
    char *value = malloc(1);
    *value = '\0';
    END_PROFILE_FUNC();
    return value;
  }

  const uint8_t start_index = i;

  uint8_t end_index = i;
  while (i < value_width && card->string[i + card->current_index] != '\0') {
    if (card->string[i + card->current_index] != ' ') {
      end_index = i;
    }
    i++;
  }

  char *value = malloc(end_index - start_index + 1 + 1);
  memcpy(value, &card->string[start_index + card->current_index],
         end_index - start_index + 1);
  value[end_index - start_index + 1] = '\0';

  END_PROFILE_FUNC();
  return value;
}

char *card_parse_string_width_no_trim(const card_t *card, uint8_t value_width) {
  BEGIN_PROFILE_FUNC();

  char *value = malloc(value_width + 1);
  memcpy(value, &card->string[card->current_index], value_width);
  value[value_width] = '\0';

  END_PROFILE_FUNC();
  return value;
}

char *card_parse_whole(const card_t *card) {
  BEGIN_PROFILE_FUNC();

  size_t i = 0;
  while (card->string[i] == ' ') {
    i++;
  }

  const size_t start_index = i;

  size_t end_index = i;
  while (card->string[i] != '\0') {
    if (card->string[i] != ' ') {
      end_index = i;
    }
    i++;
  }

  char *value = malloc(end_index - start_index + 1 + 1);
  memcpy(value, &card->string[start_index], end_index - start_index + 1);
  value[end_index - start_index + 1] = '\0';

  END_PROFILE_FUNC();
  return value;
}

char *card_parse_whole_no_trim(const card_t *card) {
  BEGIN_PROFILE_FUNC();

  const int len = strlen(card->string);
  char *value = malloc(len + 1);
  memcpy(value, card->string, len + 1);

  END_PROFILE_FUNC();
  return value;
}