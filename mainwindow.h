#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_map_insert_button_clicked();

    void on_map_show_button_clicked();

    void on_map_erase_button_clicked();

    void on_map_clear_button_clicked();

    void on_set_insert_button_clicked();

    void on_set_show_button_clicked();

    void on_set_erase_button_clicked();

    void on_set_clear_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
