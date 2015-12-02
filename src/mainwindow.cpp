/*\
 * mainwindow.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include "mainwindow.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    hbox = new QHBoxLayout();
#ifndef USE_CODE_EDITOR
    hbox->addWidget(line_nums);
#endif // !USE_CODE_EDITOR
    hbox->addWidget(editor);
    hbox->addWidget(editor2);

    //setCentralWidget(editor);
    widget = new QWidget();
    widget->setLayout(hbox);
    setCentralWidget(widget);
    setWindowTitle(tr("Tidy Viewer"));
    readSettings();

}

void MainWindow::saveSettings()
{
    // QSettings settings("MyCompany", "MyApp");
    QSettings settings(APP_SETD, APP_SETN);
    settings.setValue("main/geometry", saveGeometry());
    settings.setValue("main/windowState", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::readSettings()
{
    // QSettings settings("MyCompany", "MyApp");
    QSettings settings(APP_SETD, APP_SETN);
    restoreGeometry(settings.value("main/geometry").toByteArray());
    restoreState(settings.value("main/windowState").toByteArray());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Tidy Viewer"),
                tr("<p>The <b>Tidy Viewer</b> example shows how "
                   "to perform simple syntax highlighting by subclassing "
                   "the QSyntaxHighlighter class and describing "
                   "highlighting rules using regular expressions.</p>"
                   "<p>Tidy Viewer has two views. The left is the <b>raw</b> HTML "
                   "file text, and the right is a <b>tidied</b> view if data "
                   "is successful obtained from libtidy</p> "
                   "<p>Version: " TIDYVIEW_VERSION " Date: " TIDYVIEW_DATE "</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}


static vLMAP lmap;

static void TIDY_CALL tidyPPProgress( TidyDoc tdoc, uint line, uint col, uint destLine )
{
    LINEMAP lm;
    lm.line = line;
    lm.col  = col;
    lm.dest = destLine;
    lmap.push_back(lm);
}

void MainWindow::getTidyView(QString data)
{
    //editor2->setPlainText(data);
    TidyDoc tdoc = tidyCreate();
    int rc;
    //Bool ok;
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};

    tidySetPrettyPrinterCallback( tdoc, tidyPPProgress);
    rc = tidySetErrorBuffer( tdoc, &errbuf );    // Capture diagnostics
    if (rc >= 0)
        rc = tidyParseString( tdoc, data.toStdString().c_str() ); // Parse the input
    if (rc >= 0)
        rc = tidyCleanAndRepair( tdoc );             // Tidy it up!
    if (rc >= 0)
        rc = tidyRunDiagnostics( tdoc );             // Kvetch
    if (rc >= 0)
        rc = tidySaveBuffer( tdoc, &output );        // Pretty Print
    if ((rc >= 0) && (output.bp) && (output.size > 0)) {
        QString s = QString::fromUtf8((const char *)output.bp);
        editor2->plm = &lmap;
        editor2->setPlainText( s );
    } else {

    }

    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease(tdoc);

}


void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), "", "HTML Files (*.html *.htm)");
            //tr("Open File"), "", "C++ Files (*.cpp *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QString data = file.readAll();
            editor->setPlainText(data);
            getTidyView(data);
        }
    }
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
#ifdef USE_CODE_EDITOR
    editor = new CodeEditor;
    editor->setFont(font);
    editor->setReadOnly(true);

    editor2 = new CodeEditor;
    editor2->setFont(font);
    editor2->setReadOnly(true);

#else // !USE_CODE_EDITOR
    editor = new QTextEdit;
    editor->setFont(font);
    editor->setReadOnly(true);

    editor2 = new QTextEdit;
    editor2->setFont(font);
    editor2->setReadOnly(true);

    line_nums = new QTextEdit;
    line_nums->setFont(font);
    line_nums->setReadOnly(true);
    line_nums->setFixedWidth(15);
    line_nums->setFrameStyle(0);
#endif // USE_CODE_EDITOR y/n

    highlighter = new Highlighter(editor->document());
    highlighter2 = new Highlighter(editor2->document());

    //openFile(file);
     //   QFile f(file);
     //   if (f.open(QFile::ReadOnly | QFile::Text))
     //       editor->setPlainText(f.readAll());
}
//! [1]

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    //fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
    //                    QKeySequence::New);

    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                        QKeySequence::Open);
                        
    QKeySequence ks(tr("Ctrl+Q"));
    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()), ks);
    //                    QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

/* eof */
