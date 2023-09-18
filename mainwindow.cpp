#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rbt.h"
#include <QMessageBox>
rbt<int, std::string> map;
rbt<int, char> set;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_map_insert_button_clicked()
{
    if(ui->map_key_insert->text().isEmpty() || ui->map_value_insert->text().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "You have empty line.");
        return;
    }
    int key = ui->map_key_insert->text().toInt();
    std::string val = ui->map_value_insert->text().toStdString();
    map.add(key,val);
}


void MainWindow::on_map_show_button_clicked()
{
    if(map.empty())
    {
        QMessageBox::warning(this, "ERROR", "Your map is empty.");
        ui->map_show->clear();
        return;
    }
    else
    {
        ui->map_show->clear();
        for(auto it = map.begin(); it!=map.end(); ++it)
        {
            ui->map_show->insertPlainText("(" + QString::number((*it).first) + ": " + QString::fromStdString((*it).second) + ")" + "\n");
        }
    }
}
void MainWindow::on_map_erase_button_clicked()
{
    if(ui->map_erase->text().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "Your line is empty.");
        return;
    }
    else
    {
        int keyt = ui->map_erase->text().toInt();
        map.remove(keyt);
    }
}


void MainWindow::on_map_clear_button_clicked()
{
    map.clear();
}


void MainWindow::on_set_insert_button_clicked()
{
    if(ui->set_key->text().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "You have empty line.");
        return;
    }
    int key = ui->set_key->text().toInt();
    char val = 'a';
    set.add(key,val);
}


void MainWindow::on_set_show_button_clicked()
{
    if(set.empty())
    {
        QMessageBox::warning(this, "ERROR", "Your set is empty.");
        ui->set_show->clear();
        return;
    }
    else
    {
        ui->set_show->clear();
        for(auto it = set.begin(); it!=set.end(); ++it)
        {
            ui->set_show->insertPlainText(QString::number((*it).first)+ "\n");
        }
    }
}


void MainWindow::on_set_erase_button_clicked()
{
    if(ui->set_key_erase->text().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "Your line is empty.");
        return;
    }
    else
    {
        int keyt = ui->set_key_erase->text().toInt();
        set.remove(keyt);
    }
}


void MainWindow::on_set_clear_button_clicked()
{
    set.clear();
}

