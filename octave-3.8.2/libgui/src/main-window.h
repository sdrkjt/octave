/*

Copyright (C) 2013 John W. Eaton
Copyright (C) 2011-2013 Jacob Dawid

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

#if !defined (octave_main_window_h)
#define octave_main_window_h 1

// Qt includes
#include <QMainWindow>
#include <QThread>
#include <QTabWidget>
#include <QMdiArea>
#include <QStatusBar>
#include <QToolBar>
#include <QQueue>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QToolButton>
#include <QComboBox>
#include <QSemaphore>

// Editor includes
#include "file-editor-interface.h"

// QTerminal includes
#include "QTerminal.h"

// Own includes
#include "dialog.h"
#include "resource-manager.h"
#include "workspace-model.h"
#include "workspace-view.h"
#include "history-dock-widget.h"
#include "files-dock-widget.h"
#include "terminal-dock-widget.h"
#include "documentation-dock-widget.h"
#include "octave-qt-link.h"
#include "octave-dock-widget.h"
#include "find-files-dialog.h"

/**
 * \class MainWindow
 *
 * Represents the main window.
 */
class main_window : public QMainWindow
{
  Q_OBJECT

public:

  typedef std::pair <std::string, std::string> name_pair;

  main_window (QWidget *parent = 0);

  ~main_window (void);

  bool command_window_has_focus (void) const;

  void focus_command_window (void);

signals:
  void settings_changed (const QSettings *);
  void new_file_signal (const QString&);
  void open_file_signal (const QString&);

  void show_doc_signal (const QString&);

  void insert_debugger_pointer_signal (const QString& file, int line);
  void delete_debugger_pointer_signal (const QString& file, int line);
  void update_breakpoint_marker_signal (bool insert, const QString& file,
                                        int line);

  void copyClipboard_signal (void);
  void pasteClipboard_signal (void);

  void set_widget_shortcuts_signal (bool);

public slots:
  void report_status_message (const QString& statusMessage);
  void handle_save_workspace_request (void);
  void handle_load_workspace_request (const QString& file = QString ());
  void handle_clear_command_window_request (void);
  void handle_clear_workspace_request (void);
  void handle_clear_history_request (void);
  void handle_rename_variable_request (const QString& old_name,
                                       const QString& new_name);
  void handle_undo_request (void);
  void new_file (const QString& commands = QString ());
  void open_file (const QString& file_name = QString ());
  void open_online_documentation_page (void);
  void display_release_notes (void);
  void load_and_display_community_news (int serial = -1);
  void display_community_news (const QString& news);
  void open_bug_tracker_page (void);
  void open_octave_packages_page (void);
  void open_agora_page (void);
  void open_contribute_page (void);
  void open_developer_page (void);
  void process_settings_dialog_request (const QString& desired_tab
                                                         = QString ());
  void show_about_octave (void);
  void notice_settings (const QSettings *settings);
  void prepare_to_exit (void);
  void exit (int status);
  void reset_windows (void);

  void hide_warning_bar (void);
  void show_gui_info (void);

  void change_directory (const QString& dir);
  void browse_for_directory (void);
  void set_current_working_directory (const QString& dir);
  void change_directory_up (void);
  void accept_directory_line_edit (void);

  void execute_command_in_terminal (const QString& dir);
  void run_file_in_terminal (const QFileInfo& info);

  void handle_new_figure_request (void);

  void handle_enter_debugger (void);
  void handle_exit_debugger (void);
  void debug_continue (void);
  void debug_step_into (void);
  void debug_step_over (void);
  void debug_step_out (void);
  void debug_quit (void);

  void handle_insert_debugger_pointer_request (const QString& file, int line);
  void handle_delete_debugger_pointer_request (const QString& file, int line);
  void handle_update_breakpoint_marker_request (bool insert,
                                                const QString& file, int line);

  void read_settings (void);
  void set_window_layout (QSettings *settings);
  void write_settings (void);
  void connect_visibility_changed (void);

  void copyClipboard (void);
  void pasteClipboard (void);

  void connect_uiwidget_links ();

  void handle_create_dialog (const QString& message, const QString& title,
                             const QString& icon, const QStringList& button,
                             const QString& defbutton,
                             const QStringList& role);

  void handle_create_listview (const QStringList& list, const QString& mode,
                               int width, int height,
                               const QIntList& initial,
                               const QString& name,
                               const QStringList& prompt,
                               const QString& ok_string,
                               const QString& cancel_string);

  void handle_create_inputlayout (const QStringList&, const QString&,
                                  const QFloatList&, const QFloatList&,
                                  const QStringList&);

  void handle_create_filedialog (const QStringList &filters,
                                 const QString& title, const QString& filename,
                                 const QString &dirname,
                                 const QString& multimode);

  void handle_show_doc (const QString &file);

  // find files dialog
  void find_files (const QString &startdir=QDir::currentPath ());
  void find_files_finished (int);

  // setting global shortcuts
  void set_global_shortcuts (bool enable);

  // handling the clipboard
  void clipboard_has_changed (QClipboard::Mode);
  void clear_clipboard ();

  // get the dockwidgets
  QList<octave_dock_widget *> get_dock_widget_list ()
    { return dock_widget_list (); }

protected:
  void closeEvent (QCloseEvent * closeEvent);

private:

  void construct (void);

  void construct_octave_qt_link (void);

  void construct_menu_bar (void);
  void construct_file_menu (QMenuBar *p);
  void construct_new_menu (QMenu *p);
  void construct_edit_menu (QMenuBar *p);
  void construct_debug_menu_item (QMenu *p, const QString& item,
                                  const QKeySequence& key);
  QAction *construct_debug_menu_item (const char *icon_file,
                                      const QString& item,
                                      const QKeySequence& key);
  void construct_debug_menu (QMenuBar *p);
  QAction *construct_window_menu_item (QMenu *p, const QString& item,
                                       bool checkable,
                                       const QKeySequence& key);
  void construct_window_menu (QMenuBar *p);
  void construct_help_menu (QMenuBar *p);
  void construct_documentation_menu (QMenu *p);

  void construct_news_menu (QMenuBar *p);

  void construct_warning_bar (void);
  void construct_gui_info_button (void);

  void construct_tool_bar (void);

  void establish_octave_link (void);

  void save_workspace_callback (const std::string& file);

  void load_workspace_callback (const std::string& file);

  void rename_variable_callback (const name_pair& names);

  void command_window_undo_callback (void);

  void clear_command_window_callback (void);

  void resize_command_window_callback (void);

  void clear_workspace_callback (void);

  void clear_history_callback (void);

  void execute_command_callback ();
  void run_file_callback (const QFileInfo& info);

  void new_figure_callback (void);

  void change_directory_callback (const std::string& directory);

  void exit_callback (void);

  void queue_command (QString command);

  void queue_debug (QString command);

  void execute_debug_callback ();

  workspace_model *_workspace_model;

  // Toolbars.
  QStatusBar *status_bar;

  // Subwindows.
  terminal_dock_widget *command_window;
  history_dock_widget *history_window;
  files_dock_widget *file_browser_window;
  documentation_dock_widget *doc_browser_window;
  file_editor_interface *editor_window;
  workspace_view *workspace_window;
  QList<octave_dock_widget *> dock_widget_list ()
  {
    QList<octave_dock_widget *> list = QList<octave_dock_widget *> ();
    list.append (static_cast<octave_dock_widget *> (command_window));
    list.append (static_cast<octave_dock_widget *> (history_window));
    list.append (static_cast<octave_dock_widget *> (file_browser_window));
    list.append (static_cast<octave_dock_widget *> (doc_browser_window));
#ifdef HAVE_QSCINTILLA
    list.append (static_cast<octave_dock_widget *> (editor_window));
#endif
    list.append (static_cast<octave_dock_widget *> (workspace_window));
    return list;
  }

  QString _release_notes_icon;

  QToolBar *_main_tool_bar;

  QDockWidget *_warning_bar;
  QPushButton *_gui_info_button;

  QMenu *_debug_menu;

  QAction *_debug_continue;
  QAction *_debug_step_into;
  QAction *_debug_step_over;
  QAction *_debug_step_out;
  QAction *_debug_quit;

  QAction *_new_script_action;
  QAction *_new_function_action;
  QAction *_open_action;

  QAction *_copy_action;
  QAction *_paste_action;
  QAction *_clear_clipboard_action;
  QAction *_undo_action;

  QAction *_find_files_action;
  QAction *_exit_action;

  // Toolbars.
  QComboBox *_current_directory_combo_box;
  static const int current_directory_width = 300;
  static const int current_directory_max_visible = 16;
  static const int current_directory_max_count = 16;
  QLineEdit *_current_directory_line_edit;

  // Find files dialog
  find_files_dialog * find_files_dlg;

  // release notes window
  QWidget * release_notes_window;

  QWidget * community_news_window;

  octave_qt_link *_octave_qt_link;

  QClipboard *_clipboard;

  // Flag for closing whole application.
  bool _closing;

  // semaphore to synchronize execution signals and related callback
  QStringList *_cmd_queue;
  QSemaphore   _cmd_processing;
  QMutex       _cmd_queue_mutex;

  // semaphore to synchronize debug signals and related callbacks
  QStringList *_dbg_queue;
  QSemaphore   _dbg_processing;
  QMutex       _dbg_queue_mutex;
};

class news_reader : public QObject
{
  Q_OBJECT

public:

  news_reader (const QString& xbase_url, const QString& xpage,
               int xserial = -1, bool xconnect_to_web = false)
    : QObject (), base_url (xbase_url), page (xpage), serial (xserial),
      connect_to_web (xconnect_to_web)
  { }

public slots:

  void process (void);

signals:

  void display_news_signal (const QString& news);

  void finished (void);

private:

  QString base_url;
  QString page;
  int serial;
  bool connect_to_web;
};

#endif // MAINWINDOW_H
