#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <sstream>
#include <QString>
#include <fstream>
#include <QFileDialog>

#define NOMBRE_ARCHIVO "Neuronas.txt"
#define DELIMITADOR_CAMPO '|'
#define DELIMITADOR_REGISTRO '\n'

QString input;
AdministradoraNeurona admiNeurona;

struct datosNeurona
{
    int id = C_0;
    int posX = C_0;
    int posY = C_0;
    int r = C_0;
    int g = C_0;
    int b = C_0;
    float voltaje = C_0;
}dNeurona;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //enableDarkMode(); //Dark mode ;)
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Métodos CLase Nerona
Neurona::Neurona(int& id, float& voltaje, int& posicion_x, int& posicion_y, int& red, int& green,
                 int& blue)
{
    if(id > 0){this->setId(id);}
    if(voltaje > 0){this->setVoltaje(voltaje);}
    if(posicion_x > 0){this->setPosicionX(posicion_x);}
    if(posicion_y > 0){this->setPosicionY(posicion_y);}
    if(red > 0){this->setRed(red);}
    if(green > 0){this->setGreen(green);}
    if(blue > 0){this->setBlue(blue);}
}
std::string Neurona::printNeurona()
{
    std::stringstream ss;
    ss <<  "ID: " << this->getId() <<std::endl
       << "Voltaje: " << this->getVoltaje() <<std::endl
       << "Posicion X: " << this->getPosicionX() <<std::endl
       << "Posicion Y: " << this->getPosicionY() <<std::endl
       << "Red: " << this->getRed() <<std::endl
       << "Green: " << this->getGreen() <<std::endl
       << "Blue: " << this->getBlue() <<std::endl <<std::endl;

    return ss.str();
}
std::string Neurona::getInfoNeurona()
{
    std::stringstream ss;

    ss << this->getId() << DELIMITADOR_CAMPO << this->getVoltaje() << DELIMITADOR_CAMPO
       << this->getPosicionX() << DELIMITADOR_CAMPO << this->getPosicionY() << DELIMITADOR_CAMPO
       << this->getRed() << DELIMITADOR_CAMPO << this->getGreen() << DELIMITADOR_CAMPO
       << this->getBlue() << DELIMITADOR_REGISTRO;

    return ss.str();
}

//Métodos Clase AdministradoraNeurona
void AdministradoraNeurona::agregarFinal(int& id, float& voltaje, int& posicion_x, int& posicion_y,
                                          int& red, int& green, int& blue)
{
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->neurona = new Neurona(id, voltaje, posicion_x, posicion_y, red, green, blue);
    nuevoNodo->siguiente = NULL;

    if(inicio == NULL){inicio = nuevoNodo;}
    else
    {
        Nodo* temp = inicio;
        while (temp->siguiente != NULL){temp = temp->siguiente;}
        temp->siguiente = nuevoNodo;
    }
    contadorNeuronas++;
}
void AdministradoraNeurona::agregarInicio(int& id, float& voltaje, int& posicion_x, int& posicion_y,
                                         int& red, int& green, int& blue)
{
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->neurona = new Neurona(id, voltaje, posicion_x, posicion_y, red, green, blue);
    nuevoNodo->siguiente = NULL;

    if(inicio == NULL){inicio = nuevoNodo;}
    else
    {
        nuevoNodo->siguiente = inicio;
        inicio = nuevoNodo;
    }
    contadorNeuronas++;
}

int AdministradoraNeurona::searchNeurona(int neuronaABuscar)
{
    Nodo* nodoTemporal = inicio;

    for(int i = 0; i<contadorNeuronas && nodoTemporal != NULL; i++)
    {
        if(nodoTemporal->neurona->getId() == neuronaABuscar){return i;}
        nodoTemporal = nodoTemporal->siguiente;
    }
    return -1;
}

void AdministradoraNeurona::recuperaNeurona(int& pos, int& id, float& voltaje, int& posX, int& posY, int& red, int& green, int& blue)
{
    Nodo* nodoTemporal = inicio;

    for(int i = 0; i<contadorNeuronas && nodoTemporal != NULL; i++)
    {
        if(i == pos)
        {
            id = nodoTemporal->neurona->getId();
            voltaje = nodoTemporal->neurona->getVoltaje();
            posX = nodoTemporal->neurona->getPosicionX();
            posY = nodoTemporal->neurona->getPosicionY();
            red = nodoTemporal->neurona->getRed();
            green = nodoTemporal->neurona->getGreen();
            blue = nodoTemporal->neurona->getBlue();
        }
        nodoTemporal = nodoTemporal->siguiente;
    }
}

int AdministradoraNeurona::getContadorNeuronas(){return contadorNeuronas;}

void AdministradoraNeurona::setContadorNeuronas(int& contador){contadorNeuronas = contador;}

QString AdministradoraNeurona::guardarNeuronas()
{
    std::ofstream archivoNeuronas;
    std::stringstream ss;

    if(contadorNeuronas > 0)
    {
        archivoNeuronas.open(NOMBRE_ARCHIVO, std::ios::out);
        if(archivoNeuronas.is_open())
        {
            Nodo* nodoTemporal = inicio;
            ss << contadorNeuronas << DELIMITADOR_CAMPO;
            for(int i = 0; i<contadorNeuronas && nodoTemporal != NULL; i++)
            {
                ss << nodoTemporal->neurona->getInfoNeurona();
                nodoTemporal = nodoTemporal->siguiente;
            }
            archivoNeuronas << ss.str();
            ss.str(std::string()); //Vacía el contenido del Stringstream
            ss << "Neuronas registradas en el archivo " << NOMBRE_ARCHIVO;

            return QString::fromStdString(ss.str());
        }
        else {ss.str(std::string()); ss << "No fue posible abrir el archivo...";}
        archivoNeuronas.close();
    }
    else{ss << "No hay neuronas registradas";}

    return QString::fromStdString(ss.str());
}

QString AdministradoraNeurona::cargarNeuronas()
{
    std::ifstream archivoNeuronas;
    int contadorNeuronas = 0;
    int id, pos_X, pos_Y, red, green, blue, pos;
    float voltaje;
    char delimCampo;
    std::string cadena, subcadena;

    QString fName = QFileDialog::getOpenFileName();

    archivoNeuronas.open(fName.toStdString());
    if(archivoNeuronas)
    {
        archivoNeuronas >> contadorNeuronas;
        archivoNeuronas >> delimCampo;
        admiNeurona.setContadorNeuronas(contadorNeuronas);
        for(int i = 0; i < contadorNeuronas; i++)
        {
            std::getline(archivoNeuronas, cadena);

            pos = cadena.find_first_of(DELIMITADOR_CAMPO, 0);
            subcadena = cadena.substr(0, pos);
            id = std::stoi(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO, 0);
            subcadena = cadena.substr(0, pos);
            voltaje = std::stof(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO);
            subcadena = cadena.substr(0, pos);
            pos_X = std::stoi(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO);
            subcadena = cadena.substr(0, pos);
            pos_Y = std::stoi(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO);
            subcadena = cadena.substr(0, pos);
            red = std::stoi(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO);
            subcadena = cadena.substr(0, pos);
            green = std::stoi(subcadena);
            cadena = cadena.substr(pos+1);
            pos = cadena.find_first_of(DELIMITADOR_CAMPO);
            subcadena = cadena.substr(0, pos);
            blue = std::stoi(subcadena);

            admiNeurona.agregarInicio(id, voltaje, pos_X, pos_Y, red, green, blue);
        }
        setContadorNeuronas(contadorNeuronas);
        return "Se han cargado las neuronas desde el archivo";
    }
    else {
        return "No se abrio el archivo";
    }
}

void MainWindow::enableDarkMode()
{
    this->setStyleSheet("background-color: #434343;");// Set background color
    // Set's labels colors
    ui->IDLabel->setStyleSheet("color: white;");
    ui->VoltLabel->setStyleSheet("color: white;");
    ui->PosLabel->setStyleSheet("color: white;");
    ui->XLabel->setStyleSheet("color: white;");
    ui->YLabel->setStyleSheet("color: white;");
    ui->ColorLabel->setStyleSheet("color: white;");
    ui->RLabel->setStyleSheet("color: white;");
    ui->GLabel->setStyleSheet("color: white;");
    ui->BLabel->setStyleSheet("color: white;");
    ui->IDSearchLabel->setStyleSheet("color: white;");
    // Set's LineEdits colors
    ui->IDLineEdit->setStyleSheet("color: white;");
    ui->VoltLineEdit->setStyleSheet("color: white;");
    ui->PosXLineEdit->setStyleSheet("color: white;");
    ui->PosYLineEdit->setStyleSheet("color: white;");
    ui->ColorRLineEdit->setStyleSheet("color: white;");
    ui->ColorGLineEdit->setStyleSheet("color: white;");
    ui->ColorBLineEdit->setStyleSheet("color: white;");
    ui->IDSearchLineEdit->setStyleSheet("color: white;");
    // Set's PushButtons colors
    ui->BegginInsertPushButton->setStyleSheet("color: white;");
    ui->EndInsertPushButton->setStyleSheet("color: white;");



}

//Interacción con la interfaz
void MainWindow::on_InserPushButton_clicked() //Recupera la información de los LineTextEdit
{
    int id, posX, posY, r, g, b;
    float voltaje;
    bool dataError;

    ui->IDLineEdit->setInputMask(0000); //Establece un InputMask para aceptar números únicamente
    ui->VoltLineEdit->setInputMask(0000);
    ui->PosXLineEdit->setInputMask(0000);
    ui->PosYLineEdit->setInputMask(0000);
    ui->ColorRLineEdit->setInputMask(0000);
    ui->ColorGLineEdit->setInputMask(0000);
    ui->ColorBLineEdit->setInputMask(0000);

    if(datosCorrectos())
    {
        id = ui->IDLineEdit->text().toInt(); //ID
        voltaje = ui->VoltLineEdit->text().toFloat(); //Voltaje
        posX = ui->PosXLineEdit->text().toInt(); //Posicion X
        posY = ui->PosYLineEdit->text().toInt(); //Posicion Y
        r = ui->ColorRLineEdit->text().toInt(); //Color Rojo
        g = ui->ColorGLineEdit->text().toInt(); //Color Verde
        b = ui->ColorBLineEdit->text().toInt(); //Color Azul

        if(ui->BegginInsertPushButton->isChecked())
        {
            admiNeurona.agregarInicio(id, voltaje, posX, posY, r, g, b);
            ui->InfoDisplay->insertPlainText("Neurona creada!\n");
        }
        else if(ui->EndInsertPushButton->isChecked())
        {
            admiNeurona.agregarFinal(id, voltaje, posX, posY, r, g, b);
            ui->InfoDisplay->insertPlainText("Neurona creada!\n");
        }
        else
        {
            ui->InfoDisplay->insertPlainText("Selecciona la ubicación en la que deseas guardar la "
                                               "neurona");
        }
        ui->IDLineEdit->clear();
        ui->VoltLineEdit->clear();
        ui->PosXLineEdit->clear();
        ui->PosYLineEdit->clear();
        ui->ColorRLineEdit->clear();
        ui->ColorGLineEdit->clear();
        ui->ColorBLineEdit->clear();
    }
    else {
        ui->InfoDisplay->setPlainText("Verifica la información, sólo se aceptan números...");
    }

}

void MainWindow::on_ShowNPushButton_clicked() //Botón Mostrar Neuronas
{
    int contadorNeuronas = admiNeurona.getContadorNeuronas();

    if(contadorNeuronas > 0)
    {
        ui->tableWidget->setEnabled(true);
        do{
            ui->tableWidget->insertRow(1);
        }while (ui->tableWidget->rowCount() < admiNeurona.getContadorNeuronas());

        for(int i = C_0; i < contadorNeuronas; i++)
        {
            admiNeurona.recuperaNeurona(i, dNeurona.id, dNeurona.voltaje, dNeurona.posX, dNeurona.posY, dNeurona.r, dNeurona.g, dNeurona.b);

            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(dNeurona.id));
            QTableWidgetItem* voltItem = new QTableWidgetItem(QString::number(dNeurona.voltaje));
            QTableWidgetItem* posXItem = new QTableWidgetItem(QString::number(dNeurona.posX));
            QTableWidgetItem* posYItem = new QTableWidgetItem(QString::number(dNeurona.posY));
            QTableWidgetItem* rItem = new QTableWidgetItem(QString::number(dNeurona.r));
            QTableWidgetItem* gItem = new QTableWidgetItem(QString::number(dNeurona.g));
            QTableWidgetItem* bItem = new QTableWidgetItem(QString::number(dNeurona.b));

            ui->tableWidget->setItem(i,0, idItem);
            ui->tableWidget->setItem(i,1, voltItem);
            ui->tableWidget->setItem(i,2, posXItem);
            ui->tableWidget->setItem(i,3, posYItem);
            ui->tableWidget->setItem(i,4, rItem);
            ui->tableWidget->setItem(i,5, gItem);
            ui->tableWidget->setItem(i,6, bItem);
        }
    }
    else
        ui->InfoDisplay->setPlainText("NO hay neuronas registradas");
}

void MainWindow::on_CleanPushNutton_clicked() //Borra el contenido de los campos LineTextEdit
{
    ui->IDLineEdit->clear();
    ui->VoltLineEdit->clear();
    ui->PosXLineEdit->clear();
    ui->PosYLineEdit->clear();
    ui->ColorRLineEdit->clear();
    ui->ColorGLineEdit->clear();
    ui->ColorBLineEdit->clear();
}

bool MainWindow::datosCorrectos() //Verifica que los valores ingresados sean todos válidos, numéricos en éste caso
{
    if(ui->IDLineEdit->hasAcceptableInput() && ui->VoltLineEdit->hasAcceptableInput() &&
       ui->PosXLineEdit->hasAcceptableInput() && ui->PosYLineEdit->hasAcceptableInput() &&
       ui->ColorRLineEdit->hasAcceptableInput() && ui->ColorGLineEdit->hasAcceptableInput() &&
       ui->ColorBLineEdit->hasAcceptableInput()) return true;
    else {
        return false;
    }
}

void MainWindow::on_SavePushButton_clicked() //Botón de Respaldar Neuronas
{
    ui->InfoDisplay->setPlainText(admiNeurona.guardarNeuronas());
}

void MainWindow::on_LoadPushButton_clicked() //Botón de Cargar Neuronas
{
    ui->InfoDisplay->setPlainText(admiNeurona.cargarNeuronas());
}

void MainWindow::on_SearchPushButton_clicked()
{
    int pos;
    ui->tableWidget->clear();
    while (ui->tableWidget->rowCount() != 1) {
        ui->tableWidget->removeRow(1);
    }

    pos = admiNeurona.searchNeurona(ui->IDSearchLineEdit->text().toInt());

    if(pos != -1)
    {
        ui->tableWidget->setEnabled(true);
        ui->InfoDisplay->setPlainText("Neurona ENCONTRADA...");
        admiNeurona.recuperaNeurona(pos, dNeurona.id, dNeurona.voltaje, dNeurona.posX, dNeurona.posY, dNeurona.r, dNeurona.g, dNeurona.b);
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(dNeurona.id));
        QTableWidgetItem* voltItem = new QTableWidgetItem(QString::number(dNeurona.voltaje));
        QTableWidgetItem* posXItem = new QTableWidgetItem(QString::number(dNeurona.posX));
        QTableWidgetItem* posYItem = new QTableWidgetItem(QString::number(dNeurona.posY));
        QTableWidgetItem* rItem = new QTableWidgetItem(QString::number(dNeurona.r));
        QTableWidgetItem* gItem = new QTableWidgetItem(QString::number(dNeurona.g));
        QTableWidgetItem* bItem = new QTableWidgetItem(QString::number(dNeurona.b));

        ui->tableWidget->setItem(0,0, idItem);
        ui->tableWidget->setItem(0,1, voltItem);
        ui->tableWidget->setItem(0,2, posXItem);
        ui->tableWidget->setItem(0,3, posYItem);
        ui->tableWidget->setItem(0,4, rItem);
        ui->tableWidget->setItem(0,5, gItem);
        ui->tableWidget->setItem(0,6, bItem);
    }
    else{ui->InfoDisplay->setPlainText("No se encontró la neurona..."); ui->tableWidget->setEnabled(false);}
}

void MainWindow::on_DibujarNPushButton_clicked()
{
    QPen pen;
    QColor colorNeurona;
    int contadorNeurona = admiNeurona.getContadorNeuronas();

    if(contadorNeurona > 0)
    {
        for(int i = 0; i < contadorNeurona; i++)
        {
            admiNeurona.recuperaNeurona(i, dNeurona.id, dNeurona.voltaje, dNeurona.posX,
                                        dNeurona.posY, dNeurona.r, dNeurona.g, dNeurona.b);
            colorNeurona.setRgb(dNeurona.r, dNeurona.g, dNeurona.b);
            escena->addEllipse(dNeurona.posX, dNeurona.posY, dNeurona.voltaje, dNeurona.voltaje, pen, QBrush(QColor(colorNeurona)));
        }
        ui->graphicsView->setScene(escena);
    }
}

void MainWindow::on_LimpiarPanPushButton_clicked()
{
    escena->clear();
}

