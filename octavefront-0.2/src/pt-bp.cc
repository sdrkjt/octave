/*

Copyright (C) 2001 Ben Sapp

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#if defined (__GNUG__)
#pragma implementation
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ov-usr-fcn.h"
#include "ov-list.h"
#include "pager.h"
#include "pt-all.h"

void 
tree_breakpoint::take_action (tree &tr)
{
  if (act == set)
    {
      tr.set_breakpoint ();
      line = tr.line ();
      found = true;
    }
  else if (act == clear)
    {
      tr.delete_breakpoint ();
      found = true;
    }
  else if (act == list)
    {
      if (tr.is_breakpoint ())
	{
	  bp_list.append (octave_value (static_cast<double> (tr.line ())));
	  line = tr.line () + 1;
	}
    }
  else
    panic_impossible ();

  return;
}

void
tree_breakpoint::visit_while_command (tree_while_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_expression *expr = cmd.condition ();

  if (expr)
    expr->accept (*this);

  tree_statement_list *list = cmd.body ();

  if (list)
    list->accept (*this);
}

void
tree_breakpoint::visit_do_until_command (tree_do_until_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_statement_list *lst = cmd.body ();

  if (lst)
    lst->accept (*this);

  if (found)
    return;

  tree_expression *expr = cmd.condition ();

  if (expr)
    expr->accept (*this);
}

void 
tree_breakpoint::visit_argument_list (tree_argument_list& lst)
{
  if (found)
    return;

  for (Pix p = lst.first (); p != 0; lst.next (p))
    {
      tree_expression *elt = lst(p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_binary_expression (tree_binary_expression& expr)
{
  if (found)
    return;

  tree_expression *lhs = expr.lhs ();
  tree_expression *rhs = expr.rhs ();

  if (lhs && lhs->line () >= line)
    lhs->accept (*this);

  if (rhs && rhs->line () >= line)
    rhs->accept (*this);  
}

void 
tree_breakpoint::visit_break_command (tree_break_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);
}

void
tree_breakpoint::visit_colon_expression (tree_colon_expression& expr)
{
  if (found)
    return;

  if (expr.line () >= line)
    take_action (expr);

  tree_expression *base = expr.base (); 

  if (base)
    base->accept (*this);

  tree_expression *increment = expr.increment (); 

  if (increment)
    increment->accept (*this);

  tree_expression *limit = expr.limit (); 

  if (limit)
    limit->accept (*this);
}

void 
tree_breakpoint::visit_continue_command (tree_continue_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);
}

void 
tree_breakpoint::visit_decl_command (tree_decl_command& cmd)
{
  if (found)
    return;

  tree_decl_init_list *init_list = cmd.initializer_list ();

  if (init_list)
    init_list->accept (*this);
}

void 
tree_breakpoint::visit_decl_elt (tree_decl_elt& cmd)
{
  if (found)
    return;

  tree_identifier *ident = cmd.ident ();

  if (ident)
    ident->accept (*this);

  tree_expression *expr = cmd.expression ();

  if (expr) 
    expr->accept (*this);

}

void 
tree_breakpoint::visit_decl_init_list (tree_decl_init_list& lst)
{
  if (found)
    return;

  for (Pix p = lst.first (); p != 0; lst.next (p))
    {
      tree_decl_elt *elt = lst(p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_simple_for_command (tree_simple_for_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_expression *expr = cmd.control_expr ();

  if (expr)
    expr->accept (*this);

  tree_statement_list *list = cmd.body ();

  if (list)
    list->accept (*this);
}

void 
tree_breakpoint::visit_complex_for_command (tree_complex_for_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_expression *expr = cmd.control_expr ();

  if (expr)
    expr->accept (*this);

  tree_statement_list *list = cmd.body ();

  if (list)
    list->accept (*this);

}

void 
tree_breakpoint::visit_octave_user_function (octave_user_function& cmd)
{
  // we should not visit octave user functions because the function we are currently 
  // in is the function where the breakpoint was requested
}

void 
tree_breakpoint::visit_octave_user_function_header (octave_user_function& cmd)
{
  // Do nothing
}

void 
tree_breakpoint::visit_octave_user_function_trailer (octave_user_function& cmd)
{
  // Do nothing
}

void 
tree_breakpoint::visit_identifier (tree_identifier& id)
{
  if (found)
    return;

  if (id.line () >= line )
    take_action (id);
}

void 
tree_breakpoint::visit_if_clause (tree_if_clause& cmd)
{
  if (found)
    return;

  tree_expression *expr = cmd.condition ();

  if (expr)
    expr->accept (*this);

  tree_statement_list *list = cmd.commands ();

  if (list)
    list->accept (*this);
}

void
tree_breakpoint::visit_if_command (tree_if_command& cmd)
{
  if (found)
    return;

  tree_if_command_list *list = cmd.cmd_list ();

  if (list)
    list->accept (*this);
}

void
tree_breakpoint::visit_if_command_list (tree_if_command_list& lst)
{
  if (found)
    return;

  for (Pix p = lst.first (); p != 0; lst.next (p))
    {
      tree_if_clause *elt = lst(p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_index_expression (tree_index_expression& cmd)
{
  if (found)
    return;

  tree_expression *expr = cmd.expression ();

  if (expr)
    if (expr->line () >= line)
      take_action (*expr);

  tree_argument_list *lst = cmd.arg_list ();

  if (lst)
    lst->accept (*this);
}

void 
tree_breakpoint::visit_indirect_ref (tree_indirect_ref& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);  
}

void 
tree_breakpoint::visit_matrix (tree_matrix& mat)
{
  if (found)
    return;

  Pix p = mat.first ();

  while (p)
    {
      tree_argument_list *elt = mat (p);
      mat.next (p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_cell (tree_cell& cell)
{
  if (found)
    return;

  Pix p = cell.first ();

  while (p)
    {
      tree_argument_list *elt = cell (p);
      cell.next (p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_multi_assignment (tree_multi_assignment& expr)
{
  if (found)
    return;

  tree_argument_list *list = expr.left_hand_side ();

  if (list)
    list->accept (*this);

  tree_expression *rhs = expr.right_hand_side ();

  if (rhs)
    rhs->accept (*this);
}

void 
tree_breakpoint::visit_no_op_command (tree_no_op_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);
}

void 
tree_breakpoint::visit_constant (tree_constant& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);
}

void 
tree_breakpoint::visit_parameter_list (tree_parameter_list& lst)
{
  if (found)
    return;

  Pix p = lst.first ();

  while (p)
    {
      tree_identifier *elt = lst(p);
      lst.next (p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_plot_command (tree_plot_command& cmd)
{
  if (found)
    return;

  // Don't bother looking at the range plot list since they must be
  // on the same line.

  if (cmd.line () >= line)
    take_action (cmd);
}

void
tree_breakpoint::visit_plot_limits (plot_limits& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void 
tree_breakpoint::visit_plot_range (plot_range& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void
tree_breakpoint::visit_postfix_expression (tree_postfix_expression& expr)
{
  if (found)
    return;

  if (expr.line () >= line)
    take_action (expr);

  tree_expression *e = expr.operand ();

  if (e)
    e->accept (*this);
}

void 
tree_breakpoint::visit_prefix_expression (tree_prefix_expression& expr)
{
  if (found)
    return;

  if (expr.line () >= line)
    take_action (expr);

  tree_expression *e = expr.operand ();

  if (e)
    e->accept (*this);    
}

void 
tree_breakpoint::visit_return_command (tree_return_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);
}

void
tree_breakpoint::visit_return_list (tree_return_list& lst)
{
  if (found)
    return;

  Pix p = lst.first ();

  while (p)
    {
      tree_index_expression *elt = lst(p);
      lst.next (p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_simple_assignment (tree_simple_assignment& expr)
{
  if (found)
    return;

  if (expr.line () >= line)
    take_action (expr);

}

void
tree_breakpoint::visit_statement (tree_statement& stmt)
{
  if (found)
    return;

  tree_command *cmd = stmt.command ();

  if (cmd)
    cmd->accept (*this);
  else
    {
      tree_expression *expr = stmt.expression ();

      if (expr)
	expr->accept (*this);
    }
}

void
tree_breakpoint::visit_statement_list (tree_statement_list& lst)
{
  if (found)
    return;

  for (Pix p = lst.first (); p != 0; lst.next (p))
    {
      tree_statement *elt = lst(p);

      if (elt)
	elt->accept (*this);
    }
}

void 
tree_breakpoint::visit_subplot (subplot& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void
tree_breakpoint::visit_subplot_axes (subplot_axes& cmd)
{
  // Do nothing.  This caser will be handled in visit_tree_plot_command.
}

void
tree_breakpoint::visit_subplot_list (subplot_list& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void 
tree_breakpoint::visit_subplot_style (subplot_style& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void
tree_breakpoint::visit_subplot_using (subplot_using& cmd)
{
  // Do nothing.  This case will be handled in visit_tree_plot_command.
}

void
tree_breakpoint::visit_switch_case (tree_switch_case& cmd)
{
  if (found)
    return;

  // Disallow breakpoints on the label.

  tree_statement_list *lst = cmd.commands ();

  if (lst)
    lst->accept (*this);
}

void 
tree_breakpoint::visit_switch_case_list (tree_switch_case_list& lst)
{
  if (found)
    return;

  Pix p = lst.first ();

  while (p)
    {
      tree_switch_case *elt = lst(p);

      if (elt)
	elt->accept (*this);

      lst.next (p);
    }
}


void 
tree_breakpoint::visit_switch_command (tree_switch_command& cmd)
{
  if (found)
    return;

  tree_expression *expr = cmd.switch_value ();

  if (expr)
    expr->accept (*this);

  tree_switch_case_list *lst = cmd.case_list ();

  if (lst)
    lst->accept (*this);
}

void
tree_breakpoint::visit_try_catch_command (tree_try_catch_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_statement_list *try_code = cmd.body ();

  if (try_code)
    try_code->accept (*this);

  tree_statement_list *catch_code = cmd.cleanup ();

  if (catch_code)
    catch_code->accept (*this);
}

void
tree_breakpoint::visit_unwind_protect_command (tree_unwind_protect_command& cmd)
{
  if (found)
    return;

  if (cmd.line () >= line)
    take_action (cmd);

  tree_statement_list *lst1 = cmd.body ();

  if (lst1)
    lst1->accept (*this);

  tree_statement_list *lst2 = cmd.cleanup ();

  if (lst2)
    lst2->accept (*this);
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
