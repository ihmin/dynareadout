/***********************************************************************************
 *                         This file is part of dynareadout
 *                    https://github.com/PucklaMotzer09/dynareadout
 ***********************************************************************************
 * Copyright (c) 2022 PucklaMotzer09
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

#include "d3plot.h"
#include <stdlib.h>
#include <string.h>

#define READ_CONTROL_DATA_PLOT_FILE_WORD(value)                                \
  plot_file.control_data.value = 0;                                            \
  d3_buffer_read_words(&plot_file.buffer, &plot_file.control_data.value, 1)
#define READ_CONTROL_DATA_WORD(value)                                          \
  d3_word value = 0;                                                           \
  d3_buffer_read_words(&plot_file.buffer, &value, 1)
#define CDA plot_file.control_data

d3plot_file d3plot_open(const char *root_file_name) {
  d3plot_file plot_file;
  CDA.title = NULL;
  plot_file.error_string = NULL;
  plot_file.data_pointers = NULL;
  plot_file.num_states = 0;

  plot_file.buffer = d3_buffer_open(root_file_name);
  if (plot_file.buffer.error_string) {
    plot_file.error_string = malloc(strlen(plot_file.buffer.error_string) + 1);
    memcpy(plot_file.error_string, plot_file.buffer.error_string,
           strlen(plot_file.buffer.error_string) + 1);
    return plot_file;
  }

  /* Read Title*/
  CDA.title = malloc(10 * plot_file.buffer.word_size + 1);
  d3_buffer_read_words(&plot_file.buffer, CDA.title, 10);
  CDA.title[10 * plot_file.buffer.word_size] = '\0';

  READ_CONTROL_DATA_PLOT_FILE_WORD(run_time);

  READ_CONTROL_DATA_WORD(file_type);
  if (file_type > 1000) {
    file_type -= 1000;
    /* TODO: all external(users) numbers (Node, Element, Material and Rigid
  Surface Nodes) will be written in I8 format.*/
  }
  if (file_type != D3_FILE_TYPE_D3PLOT) {
    plot_file.error_string = malloc(50);
    sprintf(plot_file.error_string, "Wrong file type: %s",
            _d3plot_get_file_type_name(file_type));
    return plot_file;
  }

  READ_CONTROL_DATA_PLOT_FILE_WORD(source_version);
  READ_CONTROL_DATA_PLOT_FILE_WORD(release_version);
  READ_CONTROL_DATA_PLOT_FILE_WORD(version);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ndim);
  READ_CONTROL_DATA_PLOT_FILE_WORD(numnp);
  READ_CONTROL_DATA_PLOT_FILE_WORD(icode);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nglbv);
  READ_CONTROL_DATA_PLOT_FILE_WORD(it);
  READ_CONTROL_DATA_PLOT_FILE_WORD(iu);
  READ_CONTROL_DATA_PLOT_FILE_WORD(iv);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ia);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nel8);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nummat8);
  READ_CONTROL_DATA_PLOT_FILE_WORD(numds);
  READ_CONTROL_DATA_PLOT_FILE_WORD(numst);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nv3d);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nel2);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nummat2);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nv1d);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nel4);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nummat4);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nv2d);
  READ_CONTROL_DATA_PLOT_FILE_WORD(neiph);
  READ_CONTROL_DATA_PLOT_FILE_WORD(neips);

  if (plot_file.buffer.word_size == 4) {
    int32_t maxint32;
    d3_buffer_read_words(&plot_file.buffer, &maxint32, 1);
    CDA.maxint = maxint32;
  } else {
    d3_buffer_read_words(&plot_file.buffer, &CDA.maxint, 1);
  }

  /*READ_CONTROL_DATA_PLOT_FILE_WORD(edlopt); Not used in LS-Dyna?*/
  READ_CONTROL_DATA_PLOT_FILE_WORD(nmsph);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ngpsph);
  READ_CONTROL_DATA_PLOT_FILE_WORD(narbs);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nelt);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nummatt);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nv3dt);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ioshl[0]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ioshl[1]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ioshl[2]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ioshl[3]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ialemat);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ncfdv1);
  READ_CONTROL_DATA_PLOT_FILE_WORD(ncfdv2);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nadapt);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nmmat);
  READ_CONTROL_DATA_PLOT_FILE_WORD(numfluid);
  READ_CONTROL_DATA_PLOT_FILE_WORD(inn);
  READ_CONTROL_DATA_PLOT_FILE_WORD(npefg);
  READ_CONTROL_DATA_PLOT_FILE_WORD(nel48);
  READ_CONTROL_DATA_PLOT_FILE_WORD(idtdt);
  READ_CONTROL_DATA_PLOT_FILE_WORD(extra);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[0]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[1]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[2]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[3]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[4]);
  READ_CONTROL_DATA_PLOT_FILE_WORD(words[5]);

  if (CDA.extra > 0) {
    READ_CONTROL_DATA_PLOT_FILE_WORD(nel20);
    READ_CONTROL_DATA_PLOT_FILE_WORD(nt3d);
  } else {
    CDA.nel20 = 0;
    CDA.nt3d = 0;
  }

  if (CDA.ndim == 5 || CDA.ndim == 7) {
    CDA.mattyp = 1;
    CDA.ndim = 3;
  } else {
    CDA.mattyp = 0;
    if (CDA.ndim == 3) {
      CDA.element_connectivity_packed = 1;
    } else {
      CDA.element_connectivity_packed = 0;
      if (CDA.ndim == 4) {
        CDA.ndim = 3;
      }
    }
  }

  size_t i = 0;
  while (i < 4) {
    CDA.ioshl[i] -= 999 * (CDA.ioshl[i] == 1000);

    i++;
  }

  if (_get_nth_digit(CDA.idtdt, 0) == 1) {
    /* TODO: An array of dT/dt values of
             length NUMNP. Array is
             written after node temperature
             arrays.*/
  }
  if (_get_nth_digit(CDA.idtdt, 1) == 1) {
    /* TODO: An array of residual forces of
             length 3*NUMNP followed by
             residual moments of length
             3*NUMNP. This data is written
             after node temperatures or
             dT/dt values if there are output.*/
  }
  if (_get_nth_digit(CDA.idtdt, 2) == 1) {
    /* TODO: Plastic strain tensor is written
             for each solid and shell after
             standard element data. For
             solids (6 values) and for shells
             (6 x 3 = 18 values), at the
             lower, middle and upper
             integration location.*/
    CDA.plastic_strain_tensor_written = 1;
  } else {
    CDA.plastic_strain_tensor_written = 0;
  }
  if (_get_nth_digit(CDA.idtdt, 3) == 1) {
    /* TODO: Thermal strain tensor is written
             after standard element data. For
             solid (6 values) and shell (6
             values) and after the plastic
             strain tensor if output.*/
    CDA.thermal_strain_tensor_written = 1;
  } else {
    CDA.thermal_strain_tensor_written = 0;
  }

  if (CDA.plastic_strain_tensor_written || CDA.thermal_strain_tensor_written) {
    CDA.istrn = _get_nth_digit(CDA.idtdt, 4);
  }

  if (CDA.maxint >= 0) {
    CDA.mdlopt = 0;
  } else if (CDA.maxint < -10000) {
    CDA.mdlopt = 2;
    CDA.maxint = CDA.maxint * -1 - 10000;
  } else if (CDA.maxint < 0) {
    CDA.mdlopt = 1;
    CDA.maxint *= -1;
  } else {
    plot_file.error_string = malloc(40);
    sprintf(plot_file.error_string, "Invalid value for MAXINT: %d", CDA.maxint);
    return plot_file;
  }

  if (CDA.idtdt < 100) {
    /* We need to compute istrn*/
    /*ISTRN can only be computed as follows and if NV2D > 0.
      If NV2D-MAXINT*(6*IOSHL(1)+IOSHL(2)+NEIPS)+8*IOSHL(3)+4*IOSHL(4) > 1
      Then ISTRN = 1, else ISTRN = 0

      If ISTRN=1, and NEIPH>=6, last the 6 additional values are the six strain
      components.

      Or NELT > 0
      If NV3DT-MAXINT*(6*IOSHL(1)+IOSHL(2)+NEIPS) > 1
      Then ISTRN = 1, else ISTRN = 0*/
    const d3_word rhs =
        CDA.maxint * (6 * CDA.ioshl[0] + CDA.ioshl[1] + CDA.neips) +
        8 * CDA.ioshl[2] + 4 * CDA.ioshl[3];
    if (CDA.nv2d > rhs + 1) {
      CDA.istrn = 1;
    } else {
      CDA.istrn = 0;
    }

    if (CDA.istrn == 1 && CDA.neiph >= 6) {
      /* TODO: last the 6 additional values are the six strain*/
    }

    if (CDA.nelt > 0) {
      if ((CDA.nv3dt -
           CDA.maxint * (6 * CDA.ioshl[0] + CDA.ioshl[1] + CDA.neips)) > 1) {
        CDA.istrn = 1;
      } else {
        CDA.istrn = 0;
      }
    }
  }

  /* Allocate the first data pointers*/
  plot_file.data_pointers = malloc(D3PLT_PTR_COUNT * sizeof(size_t));
  i = 0;
  while (i < D3PLT_PTR_COUNT) {
    plot_file.data_pointers[i] = 0;

    i++;
  }

  /* We are done with CONTROL DATA now comes the real data*/

  if (CDA.mattyp) {
    plot_file.error_string = malloc(38);
    sprintf(plot_file.error_string, "MATERIAL TYPE DATA is not implemented");
    return plot_file;
  }
  if (CDA.ialemat) {
    plot_file.error_string = malloc(42);
    sprintf(plot_file.error_string,
            "FLUID MATERIAL ID DATA is not implemented");
    return plot_file;
  }
  if (CDA.nmsph) {
    plot_file.error_string = malloc(68);
    sprintf(
        plot_file.error_string,
        "SMOOTH PARTICLE HYDRODYNAMICS ELEMENT DATA FLAGS is not implemented");
    return plot_file;
  }
  if (CDA.npefg) {
    plot_file.error_string = malloc(33);
    sprintf(plot_file.error_string, "PARTICLE DATA is not implemented");
    return plot_file;
  }

  if (!_d3plot_read_geometry_data(&plot_file)) {
    return plot_file;
  }

  if (!_d3plot_read_user_identification_numbers(&plot_file)) {
    return plot_file;
  }

  if (!_d3plot_read_extra_node_connectivity(&plot_file)) {
    return plot_file;
  }

  if (!_d3plot_read_adapted_element_parent_list(&plot_file)) {
    return plot_file;
  }

  if (CDA.nmsph > 0) {
    plot_file.error_string = malloc(72);
    sprintf(plot_file.error_string, "SMOOTH PARTICLE HYDRODYNAMICS NODE AND "
                                    "MATERIAL LIST is not implemented");
    return plot_file;
  }

  if (CDA.npefg > 0) {
    plot_file.error_string = malloc(42);
    sprintf(plot_file.error_string,
            "PARTICLE GEOMETRY DATA is not implemented");
    return plot_file;
  }

  if (CDA.ndim > 5) {
    plot_file.error_string = malloc(43);
    sprintf(plot_file.error_string,
            "RIGID ROAD SURFACE DATA is not implemented");
    return plot_file;
  }

  /* Read EOF marker (D3_EOF)*/
  double eof_marker;
  d3_buffer_read_double_word(&plot_file.buffer, &eof_marker);

  if (eof_marker != D3_EOF) {
    plot_file.error_string = malloc(30);
    sprintf(plot_file.error_string, "Here should be the EOF marker");
    return plot_file;
  }

  if (!_d3plot_read_header(&plot_file)) {
    return plot_file;
  }

  if (CDA.ncfdv1 == 67108864) {
    plot_file.error_string = malloc(36);
    sprintf(plot_file.error_string, "EXTRA DATA TYPES is not implemented");
    return plot_file;
  }

  if (!d3_buffer_next_file(&plot_file.buffer)) {
    plot_file.error_string = malloc(14);
    sprintf(plot_file.error_string, "Too few files");
    return plot_file;
  }

  /* Here comes the STATE DATA*/

  int result = 1;
  while (result) {
    result = _d3plot_read_state_data(&plot_file);
    if (result == 2) {
      if (!d3_buffer_next_file(&plot_file.buffer)) {
        break;
      }
    }
  }

  return plot_file;
}

void d3plot_close(d3plot_file *plot_file) {
  d3_buffer_close(&plot_file->buffer);

  free(plot_file->control_data.title);
  free(plot_file->data_pointers);
  free(plot_file->error_string);

  plot_file->control_data.title = NULL;
  plot_file->num_states = 0;
}

d3_word *d3plot_read_node_ids(d3plot_file *plot_file, size_t *num_ids) {
  return _d3plot_read_ids(plot_file, num_ids, D3PLT_PTR_NODE_IDS,
                          plot_file->control_data.numnp);
}

d3_word *d3plot_read_solid_element_ids(d3plot_file *plot_file,
                                       size_t *num_ids) {
  return _d3plot_read_ids(plot_file, num_ids, D3PLT_PTR_EL8_IDS,
                          plot_file->control_data.nel8);
}

d3_word *d3plot_read_beam_element_ids(d3plot_file *plot_file, size_t *num_ids) {
  return _d3plot_read_ids(plot_file, num_ids, D3PLT_PTR_EL2_IDS,
                          plot_file->control_data.nel2);
}

d3_word *d3plot_read_shell_element_ids(d3plot_file *plot_file,
                                       size_t *num_ids) {
  return _d3plot_read_ids(plot_file, num_ids, D3PLT_PTR_EL4_IDS,
                          plot_file->control_data.nel4);
}

d3_word *d3plot_read_solid_shell_element_ids(d3plot_file *plot_file,
                                             size_t *num_ids) {
  return _d3plot_read_ids(plot_file, num_ids, D3PLT_PTR_EL48_IDS,
                          plot_file->control_data.nel48);
}

d3_word *d3plot_read_all_element_ids(d3plot_file *plot_file, size_t *num_ids,
                                     int sort) {
  d3_word *all_ids = NULL;
  *num_ids = 0;
  size_t offset = 0;

  size_t num_ids_buffer;
  d3_word *ids_buffer =
      d3plot_read_solid_element_ids(plot_file, &num_ids_buffer);
  if (num_ids_buffer > 0) {
    *num_ids += num_ids_buffer;
    all_ids = realloc(all_ids, *num_ids * sizeof(d3_word));
    memcpy(&all_ids[offset], ids_buffer, num_ids_buffer * sizeof(d3_word));
    offset += num_ids_buffer;
    free(ids_buffer);
  }

  ids_buffer = d3plot_read_beam_element_ids(plot_file, &num_ids_buffer);
  if (num_ids_buffer > 0) {
    *num_ids += num_ids_buffer;
    all_ids = realloc(all_ids, *num_ids * sizeof(d3_word));
    memcpy(&all_ids[offset], ids_buffer, num_ids_buffer * sizeof(d3_word));
    offset += num_ids_buffer;
    free(ids_buffer);
  }

  ids_buffer = d3plot_read_shell_element_ids(plot_file, &num_ids_buffer);
  if (num_ids_buffer > 0) {
    *num_ids += num_ids_buffer;
    all_ids = realloc(all_ids, *num_ids * sizeof(d3_word));
    memcpy(&all_ids[offset], ids_buffer, num_ids_buffer * sizeof(d3_word));
    offset += num_ids_buffer;
    free(ids_buffer);
  }

  ids_buffer = d3plot_read_solid_shell_element_ids(plot_file, &num_ids_buffer);
  if (num_ids_buffer > 0) {
    *num_ids += num_ids_buffer;
    all_ids = realloc(all_ids, *num_ids * sizeof(d3_word));
    memcpy(&all_ids[offset], ids_buffer, num_ids_buffer * sizeof(d3_word));
    offset += num_ids_buffer;
    free(ids_buffer);
  }

  /* TODO: We can take advantage of the fact that the ids are already sorted and
   * so achieve much higher performance */
  if (sort)
    _quick_sort(all_ids, 0, *num_ids - 1);

  return all_ids;
}

double *d3plot_read_node_coordinates(d3plot_file *plot_file, size_t state,
                                     size_t *num_nodes) {
  return _d3plot_read_node_data(plot_file, state, num_nodes,
                                D3PLT_PTR_STATE_NODE_COORDS);
}

double *d3plot_read_node_velocity(d3plot_file *plot_file, size_t state,
                                  size_t *num_nodes) {
  return _d3plot_read_node_data(plot_file, state, num_nodes,
                                D3PLT_PTR_STATE_NODE_VEL);
}

double *d3plot_read_node_acceleration(d3plot_file *plot_file, size_t state,
                                      size_t *num_nodes) {
  return _d3plot_read_node_data(plot_file, state, num_nodes,
                                D3PLT_PTR_STATE_NODE_ACC);
}

double d3plot_read_time(d3plot_file *plot_file, size_t state) {
  if (state >= plot_file->num_states) {
    plot_file->error_string = malloc(70);
    sprintf(plot_file->error_string, "%d is out of bounds for the states");
    return -1.0;
  }

  double time;
  if (plot_file->buffer.word_size == 4) {
    float time32;
    d3_buffer_read_words_at(&plot_file->buffer, &time32, 1,
                            plot_file->data_pointers[D3PLT_PTR_STATES + state] +
                                plot_file->data_pointers[D3PLT_PTR_STATE_TIME]);
    time = time32;
  } else {
    d3_buffer_read_words_at(&plot_file->buffer, &time, 1,
                            plot_file->data_pointers[D3PLT_PTR_STATES + state] +
                                plot_file->data_pointers[D3PLT_PTR_STATE_TIME]);
  }

  return time;
}

const char *_d3plot_get_file_type_name(d3_word file_type) {
  switch (file_type) {
  case D3_FILE_TYPE_D3PLOT:
    return "d3plot";
  case D3_FILE_TYPE_D3DRLF:
    return "d3drlf";
  case D3_FILE_TYPE_D3THDT:
    return "d3thdt";
  case D3_FILE_TYPE_INTFOR:
    return "intfor";
  case D3_FILE_TYPE_D3PART:
    return "d3part";
  case D3_FILE_TYPE_BLSTFOR:
    return "blstfor";
  case D3_FILE_TYPE_D3CPM:
    return "d3cpm";
  case D3_FILE_TYPE_D3ALE:
    return "d3ale";
  case D3_FILE_TYPE_D3EIGV:
    return "d3eigv";
  case D3_FILE_TYPE_D3MODE:
    return "d3mode";
  case D3_FILE_TYPE_D3ITER:
    return "d3iter";
  case D3_FILE_TYPE_D3SSD:
    return "d3ssd";
  case D3_FILE_TYPE_D3SPCM:
    return "d3spcm";
  case D3_FILE_TYPE_D3PSD:
    return "d3psd";
  case D3_FILE_TYPE_D3RMS:
    return "d3rms";
  case D3_FILE_TYPE_D3FTG:
    return "d3ftg";
  case D3_FILE_TYPE_D3ACS:
    return "d3acs";
  default:
    return "invalid";
  };
}

int _get_nth_digit(d3_word value, int n) {
  int i = 0;
  while (1) {
    d3_word last_value = value;
    value /= 10;
    if (i == n) {
      value *= 10;
      return last_value - value;
    }

    i++;
  }
}

double *_d3plot_read_node_data(d3plot_file *plot_file, size_t state,
                               size_t *num_nodes, size_t data_type) {
  if (state >= plot_file->num_states) {
    plot_file->error_string = malloc(70);
    sprintf(plot_file->error_string, "%d is out of bounds for the states");
    return NULL;
  }

  *num_nodes = plot_file->control_data.numnp;
  double *coords = malloc(*num_nodes * 3 * sizeof(double));

  if (plot_file->buffer.word_size == 4) {
    float *coords32 = malloc(*num_nodes * 3 * sizeof(float));
    d3_buffer_read_words_at(&plot_file->buffer, coords32, *num_nodes * 3,
                            plot_file->data_pointers[D3PLT_PTR_STATES + state] +
                                plot_file->data_pointers[data_type]);
    size_t i = 0;
    while (i < *num_nodes) {
      coords[i * 3 + 0] = coords32[i * 3 + 0];
      coords[i * 3 + 1] = coords32[i * 3 + 1];
      coords[i * 3 + 2] = coords32[i * 3 + 2];

      i++;
    }

    free(coords32);
  } else {
    d3_buffer_read_words_at(&plot_file->buffer, coords, *num_nodes * 3,
                            plot_file->data_pointers[D3PLT_PTR_STATES + state] +
                                plot_file->data_pointers[data_type]);
  }

  return coords;
}

d3_word *_d3plot_read_ids(d3plot_file *plot_file, size_t *num_ids,
                          size_t data_type, size_t num_ids_value) {
  *num_ids = num_ids_value;
  if (num_ids_value == 0) {
    return NULL;
  }

  d3_word *ids = malloc(*num_ids * sizeof(d3_word));
  if (plot_file->buffer.word_size == 4) {
    uint32_t *ids32 = malloc(*num_ids * plot_file->buffer.word_size);
    d3_buffer_read_words_at(&plot_file->buffer, ids32, *num_ids,
                            plot_file->data_pointers[data_type]);
    size_t i = 0;
    while (i < *num_ids) {
      ids[i + 0] = ids32[i + 0];
      if (i < *num_ids - 1)
        ids[i + 1] = ids32[i + 1];
      if (i < *num_ids - 2)
        ids[i + 2] = ids32[i + 2];
      if (i < *num_ids - 3)
        ids[i + 3] = ids32[i + 3];

      i += 4;
    }

    free(ids32);
  } else {
    d3_buffer_read_words_at(&plot_file->buffer, ids, *num_ids,
                            plot_file->data_pointers[data_type]);
  }

  return ids;
}

#define SWAP(lhs, rhs)                                                         \
  d3_word temp = lhs;                                                          \
  lhs = rhs;                                                                   \
  rhs = temp

int _partition(d3_word *arr, int64_t low, int64_t high) {
  const d3_word pivot = arr[high];
  int64_t i = low - 1;

  int64_t j = low;
  while (j <= high - 1) {
    if (arr[j] < pivot) {
      i++;
      SWAP(arr[i], arr[j]);
    }
    j++;
  }
  SWAP(arr[i + 1], arr[high]);
  return (i + 1);
}

void _quick_sort(d3_word *arr, int64_t low, int64_t high) {
  if (low < high) {
    const int pi = _partition(arr, low, high);

    _quick_sort(arr, low, pi - 1);
    _quick_sort(arr, pi + 1, high);
  }
}