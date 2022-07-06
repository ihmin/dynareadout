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

#ifndef D3PLOT_H
#define D3PLOT_H
#include "d3_buffer.h"
#include "d3_defines.h"

#define D3PLT_PTR_NODE_COORDS 0
#define D3PLT_PTR_NODE_IDS 1
#define D3PLT_PTR_EL8_IDS 2
#define D3PLT_PTR_EL2_IDS 3
#define D3PLT_PTR_EL4_IDS 4
#define D3PLT_PTR_EL48_IDS 5
#define D3PLT_PTR_STATE_TIME 6
#define D3PLT_PTR_STATE_NODE_COORDS 7
#define D3PLT_PTR_STATE_NODE_VEL 8
#define D3PLT_PTR_STATE_NODE_ACC 9
#define D3PLT_PTR_STATES 10
#define D3PLT_PTR_COUNT D3PLT_PTR_STATES

typedef struct {
  struct {
    char *title;
    d3_word run_time, source_version, release_version, version, ndim, numnp,
        icode, nglbv, it, iu, iv, ia, nummat8, numds, numst, nv3d, nel2,
        nummat2, nv1d, nel4, nummat4, nv2d, neiph, neips, edlopt, nmsph, ngpsph,
        narbs, nelt, nummatt, nv3dt, ioshl[4], ialemat, ncfdv1, ncfdv2, nadapt,
        nmmat, numfluid, inn, npefg, nel48, idtdt, extra, words[6], nel20, nt3d;
    d3_word numrbs;
    int64_t nel8, maxint;
    int mattyp, istrn, plastic_strain_tensor_written,
        thermal_strain_tensor_written, element_connectivity_packed;
    uint8_t mdlopt;
  } control_data;

  size_t *data_pointers;
  size_t num_states;

  d3_buffer buffer;
  char *error_string;
} d3plot_file;

#ifdef __cplusplus
extern "C" {
#endif

d3plot_file d3plot_open(const char *root_file_name);
void d3plot_close(d3plot_file *plot_file);
d3_word *d3plot_read_node_ids(d3plot_file *plot_file, size_t *num_ids);
d3_word *d3plot_read_solid_element_ids(d3plot_file *plot_file, size_t *num_ids);
d3_word *d3plot_read_beam_element_ids(d3plot_file *plot_file, size_t *num_ids);
d3_word *d3plot_read_shell_element_ids(d3plot_file *plot_file, size_t *num_ids);
d3_word *d3plot_read_solid_shell_element_ids(d3plot_file *plot_file,
                                             size_t *num_ids);
/* sort .... wether to sort the resulting ids in ascending order*/
d3_word *d3plot_read_all_element_ids(d3plot_file *plot_file, size_t *num_ids,
                                     int sort);
/* Returns an array containing all axes of all nodes at a given state. See:
 * XYZXYZXYZXYZ...*/
double *d3plot_read_node_coordinates(d3plot_file *plot_file, size_t state,
                                     size_t *num_nodes);
double *d3plot_read_node_velocity(d3plot_file *plot_file, size_t state,
                                  size_t *num_nodes);
double *d3plot_read_node_acceleration(d3plot_file *plot_file, size_t state,
                                      size_t *num_nodes);
double d3plot_read_time(d3plot_file *plot_file, size_t state);

/***** Data sections *******/
int _d3plot_read_geometry_data(d3plot_file *plot_file);
int _d3plot_read_extra_node_connectivity(d3plot_file *plot_file);
int _d3plot_read_adapted_element_parent_list(d3plot_file *plot_file);
int _d3plot_read_header(d3plot_file *plot_file);
int _d3plot_read_user_identification_numbers(d3plot_file *plot_file);
int _d3plot_read_state_data(d3plot_file *plot_file);
/***************************/

const char *_d3plot_get_file_type_name(d3_word file_type);
int _get_nth_digit(d3_word value, int n);
double *_d3plot_read_node_data(d3plot_file *plot_file, size_t state,
                               size_t *num_nodes, size_t data_type);
d3_word *_d3plot_read_ids(d3plot_file *plot_file, size_t *num_ids,
                          size_t data_type, size_t num_ids_value);
int _partition(d3_word *arr, int64_t low, int64_t high);
void _quick_sort(d3_word *arr, int64_t low, int64_t high);

#ifdef __cplusplus
}
#endif

#endif