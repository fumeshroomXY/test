#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class projectTree{

public:
    QString projectName;
    QString projectPath;
    QString cmakeFile;
    QStringList sourceFiles;
    QStringList headerFiles;

    projectTree(QString proName, QString proPath){
        projectName = proName;
        projectPath = proPath;
        cmakeFile = "CMakeList.txt";
        sourceFiles.append("main.cpp");
    }
};

class QErrorMessage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createProject();

    void updateModelView();

signals:
    void projectsChanged();

private:
    Ui::MainWindow *ui;
    QList<projectTree*> projects;

    QStandardItemModel model;

    QErrorMessage *errordlg;
};

#endif // MAINWINDOW_H
