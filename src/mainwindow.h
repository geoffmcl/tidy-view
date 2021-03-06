/*\
 * mainwindow.h
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <tidy.h>
#include <tidybuffio.h>
#include "highlighter.h"
#include "codeeditor.h"

#ifndef TIDYVIEW_VERSION
#define TIDYVIEW_VERSION "BAD_VERSION!"
#endif
#ifndef TIDYVIEW_DATE
#define TIDYVIEW_DATE "1970.01.01"
#endif

#ifndef APP_SETD
#define APP_SETD "geoffair.org"
#endif
#ifndef APP_SETO
#define APP_SETO "geoffair"
#endif

#ifndef APP_SETN
#define APP_SETN "tidy-view"
#endif
#ifndef APP_SETV
#define APP_SETV TIDYVIEW_VERSION
#endif

#define USE_CODE_EDITOR
class CodeEditor;

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);
    void moveEvent(QMoveEvent *);

    void readSettings();
    void saveSettings();

    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    QWidget *widget;
#ifdef USE_CODE_EDITOR
    CodeEditor *editor;
    CodeEditor *editor2;
#else // !USE_CODE_EDITOR
    QTextEdit *editor;
    QTextEdit *editor2;
    QTextEdit *line_nums;
#endif // USE_CODE_EDITOR y/n

    Highlighter *highlighter;
    Highlighter *highlighter2;
    QHBoxLayout *hbox;
    void getTidyView(QString data);
    QString file;

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());


};

#endif // MAINWINDOW_H
