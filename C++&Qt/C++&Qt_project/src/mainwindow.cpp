#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QDebug>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , angle1(45), angle2(0), power(10),
    vx(0), vy(0), vz(0), x(0), y(0), z(0), t(0),
    enemyAngle1(0), enemyAngle2(0), enemyPower(0),
    evx(0), evy(0), evz(0), ex(0), ey(0), ez(0), et(0),
    g(9.8), wallZ(50.0), gameOver(false)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);

    if (!udpSocket->bind(12345, QUdpSocket::ShareAddress)) {
        qDebug() << "UDP 바인딩 실패!";
    } else {
        qDebug() << "UDP 수신 대기중 (포트 12345)";
    }

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::readPendingDatagrams);

    // QGraphicsScene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    scene->setSceneRect(-300, -300, 600, 600);

    // 벽 3x3
    int cellW = 100;
    int cellH = 50;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int x0 = -150 + col * cellW;
            int y0 = -150 + row * cellH;
            wallCells[row][col] = scene->addRect(
                x0, y0, cellW, cellH,
                QPen(Qt::black), QBrush(Qt::white));
            board[row][col] = 0; // 빙고 보드 초기화
        }
    }

    // 내 공 시작 위치 (오른쪽 하단)
    enemyX = 200;
    enemyY = 150;
    enemyBall = scene->addEllipse(-5, -5, 10, 10, QPen(Qt::black), QBrush(Qt::red));
    enemyBall->setPos(enemyX, enemyY);

    // 상대 공 시작 위치 (왼쪽 하단)
    startX = -200;
    startY = 150;
    ball = scene->addEllipse(-5, -5, 10, 10, QPen(Qt::black), QBrush(Qt::green));
    ball->setPos(startX, startY);

    // 타이머
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateBall);

    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &MainWindow::updateEnemyBall);

    spacePressed = false;

    powerTimer = new QTimer(this);
    connect(powerTimer, &QTimer::timeout, this, [=]() {
        if (spacePressed) {
            power += 2;
            if (power > 200) power = 200;
            ui->sliderPower->setValue(power);
        }
    });
    powerTimer->setInterval(10);

    // Fire 버튼
    connect(ui->btnFire, &QPushButton::pressed, this, [=]() {
        if (gameOver) return;
        spacePressed = true;
        powerTimer->start();
    });

    connect(ui->btnFire, &QPushButton::released, this, [=]() {
        if (gameOver) return;
        spacePressed = false;
        powerTimer->stop();
        fireBall();
        power = 10;
        ui->sliderPower->setValue(power);
    });

    connect(ui->sliderAngle1, &QSlider::valueChanged, this, [=](int val){
        angle1 = val;
        ui->labelAngle1->setText(QString("각도1: %1°").arg(val));
    });
    connect(ui->sliderAngle2, &QSlider::valueChanged, this, [=](int val){
        angle2 = val;
        ui->labelAngle2->setText(QString("각도2: %1°").arg(val));
    });
    connect(ui->sliderPower, &QSlider::valueChanged, this, [=](int val){
        power = val;
        ui->labelPower->setText(QString("파워: %1").arg(val));
    });

    // 기본값
    ui->sliderAngle1->setRange(0, 90);
    ui->sliderAngle1->setValue(45);
    ui->sliderAngle2->setRange(-90, 90);
    ui->sliderAngle2->setValue(0);
    ui->sliderPower->setRange(10, 200);
    ui->sliderPower->setValue(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fireBall()
{
    if (gameOver) {
        qDebug() << "게임 종료 상태 - 발사 불가";
        return;
    }

    qDebug() << "발사!" << angle1 << angle2 << power;

    t = 0;
    double rad1 = qDegreesToRadians(angle1);
    double rad2 = qDegreesToRadians(angle2);

    vx = power * qCos(rad1) * qSin(rad2);
    vy = power * qSin(rad1);
    vz = power * qCos(rad1) * qCos(rad2);

    x = y = z = 0;

    ball->setRect(-40, -40, 80, 80);
    ball->setPos(startX, startY);

    timer->start(10);

    // UDP 전송
    QByteArray data = QString("%1 %2 %3")
                          .arg(angle1)
                          .arg(angle2)
                          .arg(power)
                          .toUtf8();
    udpSocket->writeDatagram(data, QHostAddress("172.100.6.53"), 12347);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        angle2 -= 1;
        if (angle2 < -90) angle2 = -90;
        ui->sliderAngle2->setValue(angle2);
        break;
    case Qt::Key_D:
        angle2 += 1;
        if (angle2 > 90) angle2 = 90;
        ui->sliderAngle2->setValue(angle2);
        break;
    case Qt::Key_W:
        angle1 += 1;
        if (angle1 > 90) angle1 = 90;
        ui->sliderAngle1->setValue(angle1);
        break;
    case Qt::Key_S:
        angle1 -= 1;
        if (angle1 < 0) angle1 = 0;
        ui->sliderAngle1->setValue(angle1);
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::updateBall()
{
    x = vx * t;
    y = vy * t - 0.5 * g * t * t;
    z = vz * t;

    if (z >= wallZ) {
        timer->stop();
        if (gameOver) return;

        double hitX = startX + x*wallZ / (z + 1.0);
        double hitY = startY - y*wallZ / (z + 1.0);

        int cellW = 100;
        int cellH = 50;
        int col = (int)((hitX + 150) / cellW);
        int row = (int)((hitY + 150) / cellH);
        if(hitX<-150||hitY<-150)return;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            markCell(row, col, 1);
            if (board[row][col] == 1) wallCells[row][col]->setBrush(QBrush(Qt::green));
            else if (board[row][col] == 2) wallCells[row][col]->setBrush(QBrush(Qt::red));
        }
        return;
    }

    double c = 7.14, K = 571.4;
    double size = K / (z + c);
    if (size > 80) size = 80;
    if (size < 10) size = 10;

    t += 0.03;

    ball->setRect(-size/2, -size/2, size, size);
    ball->setPos(startX + x * wallZ / (z + 1.0), startY - y * wallZ / (z + 1.0));
}

void MainWindow::updateEnemyBall()
{
    ex = evx * et;
    ey = evy * et - 0.5 * g * et * et;
    ez = evz * et;

    if (ez >= wallZ) {
        enemyTimer->stop();
        if (gameOver) return;

        double hitX = enemyX + ex;
        double hitY = enemyY - ey;

        int cellW = 100;
        int cellH = 50;
        int col = (int)((hitX + 150) / cellW);
        int row = (int)((hitY + 150) / cellH);
        if(hitX<-150||hitY<-150)return;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            markCell(row, col, 2);
            if (board[row][col] == 1) wallCells[row][col]->setBrush(QBrush(Qt::green));
            else if (board[row][col] == 2) wallCells[row][col]->setBrush(QBrush(Qt::red));
        }
        return;
    }

    double c = 7.14, K = 571.4;
    double size = K / (ez + c);
    if (size > 80) size = 80;
    if (size < 10) size = 10;

    et += 0.03;

    enemyBall->setRect(-size/2, -size/2, size, size);
    enemyBall->setPos(enemyX + ex, enemyY - ey);
}

void MainWindow::readPendingDatagrams()
{
    if (gameOver) return;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString msg = QString::fromUtf8(datagram).trimmed();
        qDebug() << "수신:" << msg;

        QStringList parts = msg.split(" ");
        if (parts.size() == 3) {
            enemyAngle1 = parts[0].toDouble();
            enemyAngle2 = parts[1].toDouble();
            enemyPower  = parts[2].toDouble();

            double rad1 = qDegreesToRadians(enemyAngle1);
            double rad2 = qDegreesToRadians(enemyAngle2);

            evx = enemyPower * qCos(rad1) * qSin(rad2);
            evy = enemyPower * qSin(rad1);
            evz = enemyPower * qCos(rad1) * qCos(rad2);

            ex = ey = ez = 0;
            et = 0;

            enemyBall->setRect(-40, -40, 80, 80);
            enemyBall->setPos(enemyX, enemyY);

            enemyTimer->start(10);
        }
    }
}

void MainWindow::markCell(int row, int col, int owner)
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3) return;
    if (gameOver) return;

    if (board[row][col] != 0 && board[row][col] != owner) {
        qDebug() << "이미 상대가 점유한 칸:" << row << col;
        return;
    }

    board[row][col] = owner;

    if (checkBingo(owner)) {
        endGame(owner);
    }
}

bool MainWindow::checkBingo(int owner) const
{
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == owner && board[i][1] == owner && board[i][2] == owner)
            return true;
        if (board[0][i] == owner && board[1][i] == owner && board[2][i] == owner)
            return true;
    }
    if (board[0][0] == owner && board[1][1] == owner && board[2][2] == owner)
        return true;
    if (board[0][2] == owner && board[1][1] == owner && board[2][0] == owner)
        return true;

    return false;
}

void MainWindow::endGame(int winner)
{
    if (gameOver) return;
    gameOver = true;

    timer->stop();
    enemyTimer->stop();
    powerTimer->stop();
    if (ui->btnFire) ui->btnFire->setEnabled(false);

    QString msg = (winner == 1) ? "초록 팀 승! 게임 종료" : "빨간 팀 승! 게임 종료";
    qDebug() << "게임 종료" << msg;
    QMessageBox::information(this, "Game Over", msg);
}
