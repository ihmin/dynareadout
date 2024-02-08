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

#pragma once

#include "array.hpp"
#include <d3_defines.h>

namespace dro {

class D3plotShell : public d3plot_shell {
public:
  inline const Array<double> get_mid_history_variables() const noexcept {
    return Array<double>(mid.history_variables, num_history_variables, false);
  }
  inline const Array<double> get_inner_history_variables() const noexcept {
    return Array<double>(inner.history_variables, num_history_variables, false);
  }
  inline const Array<double> get_outer_history_variables() const noexcept {
    return Array<double>(outer.history_variables, num_history_variables, false);
  }
  const Array<double> get_add_ip_history_variables(size_t add_idx) const;

  inline const Array<d3plot_surface> get_add_ips() const noexcept {
    return Array<d3plot_surface>(add_ips, num_additional_integration_points,
                                 false);
  }
};

template <> Array<D3plotShell>::~Array<D3plotShell>();

class D3plotThickShellsState : public Array<d3plot_thick_shell> {
public:
  D3plotThickShellsState(D3plotThickShellsState &&rhs) noexcept;
  D3plotThickShellsState(d3plot_thick_shell *data, size_t size,
                         size_t num_history_variables,
                         bool delete_data = true) noexcept;
  ~D3plotThickShellsState() noexcept override;

  D3plotThickShellsState &operator=(D3plotThickShellsState &&rhs) noexcept;

  // Returns the number of history variables per surface
  inline size_t get_num_history_variables() const noexcept {
    return m_num_history_variables;
  }

  // Returns the history variables of the mid surface of the thick shell under
  // index
  const Array<double> get_mid_history_variables(size_t index) const;
  // Returns the history variables of the inner surface of the thick shell
  // under index
  const Array<double> get_inner_history_variables(size_t index) const;
  // Returns the history variables of the outer surface of the thick shell
  // under index
  const Array<double> get_outer_history_variables(size_t index) const;

private:
  size_t m_num_history_variables;
};

} // namespace dro
