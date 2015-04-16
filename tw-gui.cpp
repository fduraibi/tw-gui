/* This file is part of tw-gui.

tw-gui is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
any later version.

tw-gui is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with tw-gui.  If not, see <http://www.gnu.org/licenses/>. */

#include "tw-gui.h"
#include "ui_tw-gui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    proc(new QProcess(this))
{
    ui->setupUi(this);

    connect(proc, SIGNAL(started()),this, SLOT(procStarted()));
    connect(proc, SIGNAL(error(QProcess::ProcessError)),this, SLOT(procError(QProcess::ProcessError)));
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(procExited(int,QProcess::ExitStatus)));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(progStandardOutput()));
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(progStandardError()));

    tw_path = "/bin/tw";
    QFileInfo tw_file;


    if (tw_path.isEmpty()){
        // Check if "tw" exist in the same folder with this application
        tw_file.setFile("./tw");
    } else {
        tw_file.setFile(tw_path);
    }

    // check if file exists and if yes: Is it really a file and not a directory?
    if (!tw_file.exists() || !tw_file.isFile()) {
        ui->txtOutput->setTextColor(Qt::red);
        ui->txtOutput->setText("ERROR: Cannot find the 'tw' program!");

        //TODO: Disable all btns
    }

    setDefaultConsoleColor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefaultConsoleColor(){

    // Set background color
    //ui->txtOutput->setStyleSheet("QtxtOutput { background-color: rgb(0, 0, 0) ;	color: rgb(255, 255, 255);}");

    // Set text color
    ui->txtOutput->setTextColor(Qt::black);
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::procStarted()
{
    //ui->txtOutput->append("Started");
}

void MainWindow::procError(QProcess::ProcessError procError)
{
    ui->txtOutput->setTextColor(Qt::red);
    ui->txtOutput->append(tr("Error!!!"));
    ui->txtOutput->append(proc->errorString());

    switch (procError) {
    case QProcess::FailedToStart:
        ui->txtOutput->append(tr("Failed to start"));
        break;
    case QProcess::Crashed:
        ui->txtOutput->append(tr("Crashed"));
        break;
    case QProcess::Timedout:
        ui->txtOutput->append(tr("Timedout"));
        break;
    case QProcess::UnknownError:
        ui->txtOutput->append(tr("Unknown Error"));
    default:
        ui->txtOutput->append(tr("REALLY! Unknown Error"));
    }

    setDefaultConsoleColor();
}

void MainWindow::procExited(int exitCode, QProcess::ExitStatus exitStatus)
{
    //ui->txtOutput->append("Done.");
    ui->txtOutput->append(QString::number(exitCode));

    //    if ( myProcess->exitStatus() == 0)
    //    {
    //    qDebug () << "Program ran successfully";
    //    }
    //    if ( myProcess->exitStatus() == 2)
    //    {
    //    qDebug () << "Customized message";
    //    }
    //    if ( myProcess->exitStatus() == 3)
    //    {
    //    qDebug () << "Another text warning message";
    //    }

    QTextCursor cur = ui->txtOutput->textCursor();
    cur.movePosition(QTextCursor::Start);
    ui->txtOutput->setTextCursor(cur);

}

void MainWindow::progStandardOutput()
{
    QString abc = proc->readAllStandardOutput();
    ui->txtOutput->append(abc);
}

void MainWindow::progStandardError()
{
    QString abc = proc->readAllStandardError();
    ui->txtOutput->append(abc);
}

void MainWindow::on_btnTranslate_clicked()
{
    QStringList arguments;
    Execute(arguments);
}

void MainWindow::on_btnExactTranslate_clicked()
{
    QStringList arguments;
    arguments.append("--exact");
    Execute(arguments);
}

void MainWindow::on_btnSpelling_clicked()
{
    QStringList arguments;
    arguments.append("--spelling");
    Execute(arguments);
}

void MainWindow::on_btnSynonyms_clicked()
{
    QStringList arguments;
    arguments.append("--synonyms");
    arguments.append("--exact");
    Execute(arguments);
}

void MainWindow::Execute(QStringList list)
{
    ui->txtOutput->setText("");
    if (ui->txtWord->text().isEmpty()) {
        ui->txtOutput->setTextColor(Qt::red);
        ui->txtOutput->append(tr("ERROR: Type in a word to translate."));
        setDefaultConsoleColor();
    } else {
        list.append("en-ar");
        list.append(ui->txtWord->text());
        // Print the execute command and the parameters to the console before executing it, should help the user debug and find problems easily
        ui->txtOutput->setTextColor(Qt::green);
        ui->txtOutput->append("# " + tw_path + " " + list.join(" ") + "\n-------------------------\n\n");
        setDefaultConsoleColor();
        proc->start(tw_path, list);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QString myTitle = "Translate Word";
    QString myBody = tr("<b>Translate Word GUI by Fahad Alduraibi</b><br><br>"
                        "....<br>"
                        "...<br><br>"
                        "Source code: <a href='https://github.com/fduraibi/tw-gui'>https://github.com/fduraibi/tw-gui</a><br>"
                        "Developed by: Fahad Alduraibi<br>"
                        "v1.0");
    QMessageBox::about(this, myTitle, myBody);
}
