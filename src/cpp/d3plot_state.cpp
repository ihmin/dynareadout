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

#include "d3plot_state.hpp"
#include "d3plot.hpp"
#include <sstream>

namespace dro {

template <> Array<D3plotShell>::~Array<D3plotShell>() {
  if (m_delete_data && m_data) {
    d3plot_free_shells_state(reinterpret_cast<d3plot_shell *>(m_data));
    m_data = nullptr;
  }
}

const Array<double>
D3plotShell::get_add_ip_history_variables(size_t add_idx) const {
  if (add_idx >= num_additional_integration_points) {
    std::stringstream stream;
    stream << add_idx
           << " is an invalid index for additional integration points ("
           << add_idx << " >= " << num_additional_integration_points << ")";
    const auto str(stream.str());
    throw D3plot::Exception(
        D3plot::Exception::ErrorString(strdup(str.c_str())));
  }

  return Array<double>(add_ips[add_idx].history_variables,
                       num_history_variables, false);
}

D3plotThickShellsState::D3plotThickShellsState(
    D3plotThickShellsState &&rhs) noexcept {
  *this = std::move(rhs);
}

D3plotThickShellsState::D3plotThickShellsState(d3plot_thick_shell *data,
                                               size_t size,
                                               size_t num_history_variables,
                                               bool delete_data) noexcept
    : Array(data, size, delete_data),
      m_num_history_variables(num_history_variables) {}

D3plotThickShellsState::~D3plotThickShellsState() noexcept {
  if (m_data && m_delete_data) {
    d3plot_free_thick_shells_state(
        reinterpret_cast<d3plot_thick_shell *>(m_data));
    m_data = NULL;
  }
}

D3plotThickShellsState &
D3plotThickShellsState::operator=(D3plotThickShellsState &&rhs) noexcept {
  m_data = rhs.m_data;
  m_size = rhs.m_size;
  m_delete_data = rhs.m_delete_data;

  rhs.m_data = nullptr;
  rhs.m_size = 0;
  rhs.m_delete_data = false;
  return *this;
}

const Array<double>
D3plotThickShellsState::get_mid_history_variables(size_t index) const {
  return Array<double>(Array::operator[](index).mid.history_variables,
                       m_num_history_variables, false);
}

const Array<double>
D3plotThickShellsState::get_inner_history_variables(size_t index) const {
  return Array<double>(Array::operator[](index).inner.history_variables,
                       m_num_history_variables, false);
}

const Array<double>
D3plotThickShellsState::get_outer_history_variables(size_t index) const {
  return Array<double>(Array::operator[](index).outer.history_variables,
                       m_num_history_variables, false);
}

} // namespace dro
