#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QByteArray>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UDP 소켓 생성 및 바인드
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    // 로컬포트를 열어줌, 12345로 설정해놓음
    // ShareADDress: 여러 프로세스가 같은 UDP 포트 공유할 수 있게 허용
    // ReuseADDressHint: 여러 소켓이 동일한 IP/포트 조합을 사용할 수 있게 허용하는 힌트

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::readPendingDatagrams);
    // 송신되면 함수 호출해서 동작수행

    // 엔터 눌렀을 때도 전송되도록 설정
    connect(ui->lineEditMessage, &QLineEdit::returnPressed,
            this, &MainWindow::on_sendButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked() // 수신 처리하는 함수
{
    QByteArray data = ui->lineEditMessage->text().toUtf8();
    QHostAddress destIP(ui->lineEditIP->text());
    quint16 destPort = ui->lineEditPort->text().toUShort();

    if (data.isEmpty() || destIP.isNull() || destPort == 0) {
        return; // 빈 메시지나 잘못된 주소는 무시
    }

    // UDP 메시지 전송
    udpSocket->writeDatagram(data, destIP, destPort);

    // 내 메시지 출력
    QTextCursor cursor(ui->textBrowser->textCursor());
    cursor.movePosition(QTextCursor::End);// 커서 마지막으로 유지

    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight); // 오른쪽 정렬
    cursor.insertBlock(format);

    QTextCharFormat charFormat;
    charFormat.setForeground(QBrush(Qt::black)); // 글자색
    cursor.insertText(QString("나: %1").arg(ui->lineEditMessage->text()), charFormat); // %1자리에 문장 삽입

    // 입력창 비우기
    ui->lineEditMessage->clear();
}

void MainWindow::readPendingDatagrams() // 송신 처리하는 함수
{
    while (udpSocket->hasPendingDatagrams()) { // 여러 패킷을 받을 수 있게
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size()); // 실제 데이터 읽어옴

        // QTextCursor 가져오기
        QTextCursor cursor(ui->textBrowser->textCursor());
        cursor.movePosition(QTextCursor::End); // 문서 맨 끝으로 이동해 새 메시지가 계속 아래에 출력되게

        // 왼쪽 정렬
        QTextBlockFormat format;
        format.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(format);

        // 텍스트 출력
        QTextCharFormat charFormat;
        charFormat.setForeground(QBrush(Qt::blue));
        cursor.insertText(QString("상대: %1").arg(QString::fromUtf8(datagram)), charFormat);
    }
}
