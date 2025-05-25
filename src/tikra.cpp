#include "../include/tikra.hpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtGui/QFont>
#include <QtGui/QAction>

TikraMainWindow::TikraMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_settings(new QSettings("AetherDE", "Tikra", this))
    , m_timer(new QTimer(this))
    , m_stopwatchTimer(new QTimer(this))
    , m_countdownTimer(new QTimer(this))
    , m_stopwatchTime(0)
    , m_countdownTime(0)
    , m_isStopwatchRunning(false)
    , m_isCountdownRunning(false)
{
    setupMenuBar();
    setupUI();
    setupTimers();
    loadSettings();
    
    setWindowTitle("Aether Tikra");
    resize(750, 500);
    
    // Modern dark theme
    setStyleSheet(R"(
        QMainWindow {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QMenuBar {
            background-color: #353535;
            color: #ffffff;
            border-bottom: 1px solid #505050;
            padding: 4px;
        }
        QMenuBar::item {
            background: transparent;
            padding: 8px 12px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background-color: #404040;
        }
        QMenu {
            background-color: #353535;
            color: #ffffff;
            border: 1px solid #505050;
            border-radius: 6px;
            padding: 4px;
        }
        QMenu::item {
            padding: 8px 20px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background-color: #404040;
        }
        QListWidget {
            background-color: #353535;
            border: none;
            border-right: 1px solid #505050;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            outline: none;
        }
        QListWidget::item {
            padding: 15px 20px;
            border: none;
            border-bottom: 1px solid #404040;
        }
        QListWidget::item:selected {
            background-color: #404040;
            color: #ffffff;
        }
        QListWidget::item:hover {
            background-color: #3a3a3a;
        }
        QStackedWidget {
            background-color: #2b2b2b;
            border: none;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #505050;
            border-radius: 8px;
            margin-top: 1ex;
            padding-top: 10px;
            background-color: #353535;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            color: #ffffff;
        }
        QPushButton {
            background-color: #404040;
            color: white;
            border: 1px solid #555555;
            border-radius: 6px;
            padding: 12px 20px;
            font-weight: bold;
            min-width: 100px;
            min-height: 35px;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
        }
        QPushButton:pressed {
            background-color: #353535;
        }
        QPushButton:disabled {
            background-color: #2a2a2a;
            color: #666666;
        }
        QTimeEdit, QLineEdit, QSpinBox {
            background-color: #353535;
            border: 1px solid #505050;
            border-radius: 4px;
            padding: 8px;
            color: #ffffff;
            font-size: 14px;
            min-height: 25px;
        }
        QTimeEdit:focus, QLineEdit:focus, QSpinBox:focus {
            border-color: #666666;
        }
        QLabel {
            color: #ffffff;
        }
        QDialog {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QCheckBox {
            color: #ffffff;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 3px;
            border: 1px solid #505050;
            background-color: #353535;
        }
        QCheckBox::indicator:checked {
            background-color: #404040;
            border-color: #666666;
        }
    )");
}

TikraMainWindow::~TikraMainWindow()
{
    saveSettings();
}

void TikraMainWindow::setupMenuBar()
{
    // Settings menu
    QMenu* settingsMenu = menuBar()->addMenu("Settings");
    
    QAction* preferencesAction = new QAction("Preferences", this);
    connect(preferencesAction, &QAction::triggered, this, &TikraMainWindow::showPreferences);
    settingsMenu->addAction(preferencesAction);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("Help");
    
    QAction* aboutAction = new QAction("About", this);
    connect(aboutAction, &QAction::triggered, this, &TikraMainWindow::showAbout);
    helpMenu->addAction(aboutAction);
}

void TikraMainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Sidebar navigation
    m_sidebarList = new QListWidget();
    m_sidebarList->setFixedWidth(180);
    m_sidebarList->addItem("Clock");
    m_sidebarList->addItem("Alarms");
    m_sidebarList->addItem("Stopwatch");
    m_sidebarList->addItem("Timer");
    m_sidebarList->setCurrentRow(0);
    
    connect(m_sidebarList, &QListWidget::currentRowChanged, this, &TikraMainWindow::switchPage);
    
    // Content area
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->addWidget(createClockPage());
    m_stackedWidget->addWidget(createAlarmsPage());
    m_stackedWidget->addWidget(createStopwatchPage());
    m_stackedWidget->addWidget(createTimerPage());
    
    mainLayout->addWidget(m_sidebarList);
    mainLayout->addWidget(m_stackedWidget, 1);
}

QWidget* TikraMainWindow::createClockPage()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);
    layout->setContentsMargins(40, 40, 40, 40);
    
    m_timeLabel = new QLabel("00:00:00");
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont timeFont = m_timeLabel->font();
    timeFont.setPointSize(72);
    timeFont.setBold(true);
    timeFont.setFamily("monospace");
    m_timeLabel->setFont(timeFont);
    layout->addWidget(m_timeLabel);
    
    m_dateLabel = new QLabel("Monday, January 1, 2025");
    m_dateLabel->setAlignment(Qt::AlignCenter);
    QFont dateFont = m_dateLabel->font();
    dateFont.setPointSize(20);
    m_dateLabel->setFont(dateFont);
    m_dateLabel->setStyleSheet("color: #cccccc;");
    layout->addWidget(m_dateLabel);
    
    return widget;
}

QWidget* TikraMainWindow::createAlarmsPage()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);
    
    QGroupBox* addGroup = new QGroupBox("Add New Alarm");
    QVBoxLayout* addLayout = new QVBoxLayout(addGroup);
    
    QHBoxLayout* timeRow = new QHBoxLayout();
    timeRow->addWidget(new QLabel("Time:"));
    m_alarmTimeEdit = new QTimeEdit(QTime::currentTime().addSecs(3600));
    m_alarmTimeEdit->setDisplayFormat("hh:mm");
    timeRow->addWidget(m_alarmTimeEdit);
    timeRow->addStretch();
    
    QHBoxLayout* labelRow = new QHBoxLayout();
    labelRow->addWidget(new QLabel("Label:"));
    m_alarmLabelEdit = new QLineEdit("Wake up");
    m_alarmLabelEdit->setPlaceholderText("Enter alarm name");
    labelRow->addWidget(m_alarmLabelEdit);
    
    QPushButton* addBtn = new QPushButton("Add Alarm");
    connect(addBtn, &QPushButton::clicked, this, &TikraMainWindow::addAlarm);
    
    addLayout->addLayout(timeRow);
    addLayout->addLayout(labelRow);
    addLayout->addWidget(addBtn);
    layout->addWidget(addGroup);
    
    QGroupBox* listGroup = new QGroupBox("Active Alarms");
    QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
    
    m_alarmsList = new QListWidget();
    m_alarmsList->setMinimumHeight(250);
    m_alarmsList->setStyleSheet("QListWidget { border: 1px solid #505050; border-radius: 6px; padding: 8px; }");
    listLayout->addWidget(m_alarmsList);
    
    QPushButton* deleteBtn = new QPushButton("Delete Selected");
    connect(deleteBtn, &QPushButton::clicked, this, &TikraMainWindow::deleteSelectedAlarm);
    listLayout->addWidget(deleteBtn);
    
    layout->addWidget(listGroup);
    layout->addStretch();
    
    return widget;
}

QWidget* TikraMainWindow::createStopwatchPage()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(50);
    layout->setContentsMargins(40, 40, 40, 40);
    
    m_stopwatchLabel = new QLabel("00:00.00");
    m_stopwatchLabel->setAlignment(Qt::AlignCenter);
    QFont font = m_stopwatchLabel->font();
    font.setPointSize(56);
    font.setFamily("monospace");
    font.setBold(true);
    m_stopwatchLabel->setFont(font);
    m_stopwatchLabel->setStyleSheet("background: #3a3a3a; padding: 40px; border-radius: 15px; border: 2px solid #505050;");
    layout->addWidget(m_stopwatchLabel);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(30);
    
    m_stopwatchStartBtn = new QPushButton("Start");
    m_stopwatchStopBtn = new QPushButton("Stop");
    m_stopwatchResetBtn = new QPushButton("Reset");
    
    m_stopwatchStartBtn->setMinimumWidth(120);
    m_stopwatchStopBtn->setMinimumWidth(120);
    m_stopwatchResetBtn->setMinimumWidth(120);
    
    m_stopwatchStopBtn->setEnabled(false);
    
    connect(m_stopwatchStartBtn, &QPushButton::clicked, this, &TikraMainWindow::startStopwatch);
    connect(m_stopwatchStopBtn, &QPushButton::clicked, this, &TikraMainWindow::stopStopwatch);
    connect(m_stopwatchResetBtn, &QPushButton::clicked, this, &TikraMainWindow::resetStopwatch);
    
    buttonLayout->addWidget(m_stopwatchStartBtn);
    buttonLayout->addWidget(m_stopwatchStopBtn);
    buttonLayout->addWidget(m_stopwatchResetBtn);
    
    layout->addLayout(buttonLayout);
    
    return widget;
}

QWidget* TikraMainWindow::createTimerPage()
{
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(30);
    
    QGroupBox* setupGroup = new QGroupBox("Set Timer");
    QHBoxLayout* setupLayout = new QHBoxLayout(setupGroup);
    setupLayout->setAlignment(Qt::AlignCenter);
    setupLayout->setSpacing(20);
    
    setupLayout->addWidget(new QLabel("Minutes:"));
    m_timerMinutesSpinBox = new QSpinBox();
    m_timerMinutesSpinBox->setRange(0, 59);
    m_timerMinutesSpinBox->setValue(5);
    m_timerMinutesSpinBox->setMinimumWidth(80);
    setupLayout->addWidget(m_timerMinutesSpinBox);
    
    setupLayout->addWidget(new QLabel("Seconds:"));
    m_timerSecondsSpinBox = new QSpinBox();
    m_timerSecondsSpinBox->setRange(0, 59);
    m_timerSecondsSpinBox->setValue(0);
    m_timerSecondsSpinBox->setMinimumWidth(80);
    setupLayout->addWidget(m_timerSecondsSpinBox);
    
    layout->addWidget(setupGroup);
    
    QWidget* displayWidget = new QWidget();
    QVBoxLayout* displayLayout = new QVBoxLayout(displayWidget);
    displayLayout->setAlignment(Qt::AlignCenter);
    
    m_timerLabel = new QLabel("05:00");
    m_timerLabel->setAlignment(Qt::AlignCenter);
    QFont timerFont = m_timerLabel->font();
    timerFont.setPointSize(56);
    timerFont.setFamily("monospace");
    timerFont.setBold(true);
    m_timerLabel->setFont(timerFont);
    m_timerLabel->setStyleSheet("background: #3a3a3a; padding: 40px; border-radius: 15px; border: 2px solid #505050;");
    displayLayout->addWidget(m_timerLabel);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(30);
    
    m_timerStartBtn = new QPushButton("Start");
    m_timerStopBtn = new QPushButton("Stop");
    m_timerResetBtn = new QPushButton("Reset");
    
    m_timerStartBtn->setMinimumWidth(120);
    m_timerStopBtn->setMinimumWidth(120);
    m_timerResetBtn->setMinimumWidth(120);
    
    m_timerStopBtn->setEnabled(false);
    
    connect(m_timerStartBtn, &QPushButton::clicked, this, &TikraMainWindow::startTimer);
    connect(m_timerStopBtn, &QPushButton::clicked, this, &TikraMainWindow::stopTimer);
    connect(m_timerResetBtn, &QPushButton::clicked, this, &TikraMainWindow::resetTimer);
    
    buttonLayout->addWidget(m_timerStartBtn);
    buttonLayout->addWidget(m_timerStopBtn);
    buttonLayout->addWidget(m_timerResetBtn);
    
    displayLayout->addLayout(buttonLayout);
    layout->addWidget(displayWidget);
    layout->addStretch();
    
    updateTimerDisplay();
    
    return widget;
}

void TikraMainWindow::switchPage(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}

void TikraMainWindow::showPreferences()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Preferences");
    dialog.resize(400, 300);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QGroupBox* generalGroup = new QGroupBox("General");
    QVBoxLayout* generalLayout = new QVBoxLayout(generalGroup);
    
    QCheckBox* startMinimizedBox = new QCheckBox("Start minimized");
    startMinimizedBox->setChecked(m_settings->value("startMinimized", false).toBool());
    generalLayout->addWidget(startMinimizedBox);
    
    QCheckBox* confirmExitBox = new QCheckBox("Confirm on exit");
    confirmExitBox->setChecked(m_settings->value("confirmExit", true).toBool());
    generalLayout->addWidget(confirmExitBox);
    
    layout->addWidget(generalGroup);
    
    QGroupBox* alarmGroup = new QGroupBox("Alarms");
    QVBoxLayout* alarmLayout = new QVBoxLayout(alarmGroup);
    
    QCheckBox* soundEnabledBox = new QCheckBox("Enable alarm sound");
    soundEnabledBox->setChecked(m_settings->value("alarmSound", true).toBool());
    alarmLayout->addWidget(soundEnabledBox);
    
    layout->addWidget(alarmGroup);
    layout->addStretch();
    
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        m_settings->setValue("startMinimized", startMinimizedBox->isChecked());
        m_settings->setValue("confirmExit", confirmExitBox->isChecked());
        m_settings->setValue("alarmSound", soundEnabledBox->isChecked());
    }
}

void TikraMainWindow::showAbout()
{
    QMessageBox::about(this, "About Aether Tikra",
        "<h3>Aether Tikra v1.0</h3>"
        "<p>A modern clock application with alarms, stopwatch, and timer functionality.</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Digital clock with date display</li>"
        "<li>Multiple alarms support</li>"
        "<li>Precision stopwatch</li>"
        "<li>Countdown timer</li>"
        "</ul>"
        "<p>Built with Qt6 and C++</p>"
        "<p>Part of the AetherDE project</p>"
        "<p>Copyright © 2025 AetherDE Team</p>");
}

void TikraMainWindow::setupTimers()
{
    connect(m_timer, &QTimer::timeout, this, &TikraMainWindow::updateTime);
    m_timer->start(1000);
    
    connect(m_stopwatchTimer, &QTimer::timeout, this, &TikraMainWindow::updateStopwatch);
    connect(m_countdownTimer, &QTimer::timeout, this, &TikraMainWindow::updateCountdown);
    
    updateTime();
}

void TikraMainWindow::updateTime()
{
    QTime currentTime = QTime::currentTime();
    QDate currentDate = QDate::currentDate();
    
    m_timeLabel->setText(currentTime.toString("hh:mm:ss"));
    m_dateLabel->setText(currentDate.toString("dddd, MMMM d, yyyy"));
    
    checkAlarms(currentTime);
}

void TikraMainWindow::addAlarm()
{
    QTime time = m_alarmTimeEdit->time();
    QString label = m_alarmLabelEdit->text().trimmed();
    
    if (label.isEmpty()) {
        label = "Alarm";
    }
    
    QString alarmText = QString("%1 - %2").arg(time.toString("hh:mm"), label);
    m_alarmsList->addItem(alarmText);
    
    AlarmData alarm;
    alarm.time = time;
    alarm.label = label;
    alarm.enabled = true;
    m_alarms.append(alarm);
    
    m_alarmLabelEdit->clear();
}

void TikraMainWindow::deleteSelectedAlarm()
{
    int row = m_alarmsList->currentRow();
    if (row >= 0) {
        delete m_alarmsList->takeItem(row);
        m_alarms.removeAt(row);
    }
}

void TikraMainWindow::checkAlarms(const QTime& currentTime)
{
    for (int i = 0; i < m_alarms.size(); ++i) {
        const AlarmData& alarm = m_alarms[i];
        if (alarm.enabled && alarm.time.hour() == currentTime.hour() && 
            alarm.time.minute() == currentTime.minute() && currentTime.second() == 0) {
            
            QMessageBox::information(this, "Alarm", 
                QString("Alarm: %1\nTime: %2").arg(alarm.label, alarm.time.toString("hh:mm")));
            
            m_alarms.removeAt(i);
            delete m_alarmsList->takeItem(i);
            --i;
        }
    }
}

void TikraMainWindow::startStopwatch()
{
    m_isStopwatchRunning = true;
    m_stopwatchTimer->start(10);
    m_stopwatchStartBtn->setEnabled(false);
    m_stopwatchStopBtn->setEnabled(true);
}

void TikraMainWindow::stopStopwatch()
{
    m_isStopwatchRunning = false;
    m_stopwatchTimer->stop();
    m_stopwatchStartBtn->setEnabled(true);
    m_stopwatchStopBtn->setEnabled(false);
}

void TikraMainWindow::resetStopwatch()
{
    m_stopwatchTime = 0;
    updateStopwatchDisplay();
    if (!m_isStopwatchRunning) {
        m_stopwatchStartBtn->setEnabled(true);
        m_stopwatchStopBtn->setEnabled(false);
    }
}

void TikraMainWindow::updateStopwatch()
{
    m_stopwatchTime += 10;
    updateStopwatchDisplay();
}

void TikraMainWindow::updateStopwatchDisplay()
{
    int minutes = m_stopwatchTime / 60000;
    int seconds = (m_stopwatchTime % 60000) / 1000;
    int centiseconds = (m_stopwatchTime % 1000) / 10;
    
    m_stopwatchLabel->setText(QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(centiseconds, 2, 10, QChar('0')));
}

void TikraMainWindow::startTimer()
{
    m_countdownTime = m_timerMinutesSpinBox->value() * 60 + m_timerSecondsSpinBox->value();
    if (m_countdownTime > 0) {
        m_isCountdownRunning = true;
        m_countdownTimer->start(1000);
        m_timerStartBtn->setEnabled(false);
        m_timerStopBtn->setEnabled(true);
        updateTimerDisplay();
    }
}

void TikraMainWindow::stopTimer()
{
    m_isCountdownRunning = false;
    m_countdownTimer->stop();
    m_timerStartBtn->setEnabled(true);
    m_timerStopBtn->setEnabled(false);
}

void TikraMainWindow::resetTimer()
{
    m_countdownTime = m_timerMinutesSpinBox->value() * 60 + m_timerSecondsSpinBox->value();
    updateTimerDisplay();
    if (!m_isCountdownRunning) {
        m_timerStartBtn->setEnabled(true);
        m_timerStopBtn->setEnabled(false);
    }
}

void TikraMainWindow::updateCountdown()
{
    m_countdownTime--;
    updateTimerDisplay();
    
    if (m_countdownTime <= 0) {
        m_countdownTimer->stop();
        m_isCountdownRunning = false;
        m_timerStartBtn->setEnabled(true);
        m_timerStopBtn->setEnabled(false);
        
        QMessageBox::information(this, "Timer", "Timer finished!");
    }
}

void TikraMainWindow::updateTimerDisplay()
{
    int minutes = m_countdownTime / 60;
    int seconds = m_countdownTime % 60;
    
    m_timerLabel->setText(QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void TikraMainWindow::saveSettings()
{
    m_settings->setValue("geometry", saveGeometry());
    m_settings->setValue("windowState", saveState());
}

void TikraMainWindow::loadSettings()
{
    restoreGeometry(m_settings->value("geometry").toByteArray());
    restoreState(m_settings->value("windowState").toByteArray());
}