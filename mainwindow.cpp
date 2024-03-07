#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTreeView>
#include <QFileDialog>
#include <QInputDialog>
#include <QErrorMessage>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::createProject);

    errordlg = new QErrorMessage(this);

    model.setColumnCount(1);
    model.setHeaderData(0, Qt::Horizontal, " Projects");

    ui->treeView->setModel(&model);
    // 获取QHeaderView
    QHeaderView *header = ui->treeView->header();

    // 设置Header的样式
//    QString styleSheet = "QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
//                         "stop:0 #E0E0E0, stop:1 #F0F0F0); border: 1px solid gray; }";
//    header->setStyleSheet(styleSheet);

    header->setStyleSheet("QHeaderView::section { background-color: rgb(242, 242, 242); border: 1px solid gray; }");
}

void MainWindow::createProject()
{
    QString proDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    bool ok;
    QString proName = QInputDialog::getText(this, tr("Create a project"),
                                         tr("Project Name:"), QLineEdit::Normal,
                                         tr("untitled"), &ok);
    if (ok && !proName.isEmpty()){
        if(QDir::setCurrent(proDir)){
            if(QDir::current().mkdir(proName)){
                //qDebug() << QDir::currentPath();
                proDir =  QDir::current().filePath(proName);
                QDir::setCurrent(proDir);
            }else{
                errordlg->setWindowTitle(tr("Error Message"));
                errordlg->showMessage(tr("Failed to create a project directory!"));
                return;
            }
        }else{
            errordlg->setWindowTitle(tr("Error Message"));
            errordlg->showMessage(tr("Failed to change directory when creating a project!"));
            return;
        }
    }else{
        errordlg->setWindowTitle(tr("Error Message"));
        errordlg->showMessage(tr("Failed to create a project!"));
        return;
    }
    qDebug() << proName;
    projectTree* newPro = new projectTree(proName, proDir);
    projects.append(newPro);
    updateModelView();

}

void MainWindow::updateModelView()
{
    if(projects.isEmpty()) return;
    QStandardItem *parentItem = model.invisibleRootItem();
    foreach(projectTree* pro, projects){
        QStandardItem* itemProject = new QStandardItem;
        itemProject->setData(pro->projectName, Qt::DisplayRole);
        parentItem->appendRow(itemProject);
        QStandardItem* itemCmakeFile = new QStandardItem;
        itemCmakeFile->setData(pro->cmakeFile, Qt::DisplayRole);
        QStandardItem* itemSources = new QStandardItem;
        itemSources->setData("Sources", Qt::DisplayRole);
        QStandardItem* itemHeaders = new QStandardItem;
        itemHeaders->setData("Headers", Qt::DisplayRole);
        itemProject->appendRow(itemCmakeFile);
        itemProject->appendRow(itemSources);
        itemProject->appendRow(itemHeaders);

        pro->headerFiles.sort();
        foreach(const QString &headerFile, pro->headerFiles){
            QStandardItem* itemHeaderFile = new QStandardItem;
            itemHeaderFile->setData(headerFile, Qt::DisplayRole);
            itemHeaders->appendRow(itemHeaderFile);
        }

        pro->sourceFiles.sort();
        foreach(const QString &sourceFile, pro->sourceFiles){
            QStandardItem* itemSourceFile = new QStandardItem;
            itemSourceFile->setData(sourceFile, Qt::DisplayRole);
            itemSources->appendRow(itemSourceFile);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

