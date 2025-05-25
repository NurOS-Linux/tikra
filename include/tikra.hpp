#ifndef TIKRA_HPP
#define TIKRA_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QDate>
#include <QtCore/QSettings>

struct AlarmData {
    QTime time;
    QString label;
    bool enabled;
    
    AlarmData() : enabled(true) {}
};

class TikraMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TikraMainWindow(QWidget *parent = nullptr);
    ~TikraMainWindow();

private slots:
    void updateTime();
    void addAlarm();
    void deleteSelectedAlarm();
    void startStopwatch();
    void stopStopwatch();
    void resetStopwatch();
    void updateStopwatch();
    void startTimer();
    void stopTimer();
    void resetTimer();
    void updateCountdown();
    void switchPage(int index);
    void showPreferences();
    void showAbout();

private:
    void setupMenuBar();
    void setupUI();
    void setupTimers();
    QWidget* createClockPage();
    QWidget* createAlarmsPage();
    QWidget* createStopwatchPage();
    QWidget* createTimerPage();
    void checkAlarms(const QTime& currentTime);
    void updateStopwatchDisplay();
    void updateTimerDisplay();
    void saveSettings();
    void loadSettings();

    // Navigation
    QListWidget* m_sidebarList;
    QStackedWidget* m_stackedWidget;

    // Clock page
    QLabel* m_timeLabel;
    QLabel* m_dateLabel;

    // Alarms page
    QTimeEdit* m_alarmTimeEdit;
    QLineEdit* m_alarmLabelEdit;
    QListWidget* m_alarmsList;
    QList<AlarmData> m_alarms;

    // Stopwatch page
    QLabel* m_stopwatchLabel;
    QPushButton* m_stopwatchStartBtn;
    QPushButton* m_stopwatchStopBtn;
    QPushButton* m_stopwatchResetBtn;
    QTimer* m_stopwatchTimer;
    int m_stopwatchTime;
    bool m_isStopwatchRunning;

    // Timer page
    QLabel* m_timerLabel;
    QSpinBox* m_timerMinutesSpinBox;
    QSpinBox* m_timerSecondsSpinBox;
    QPushButton* m_timerStartBtn;
    QPushButton* m_timerStopBtn;
    QPushButton* m_timerResetBtn;
    QTimer* m_countdownTimer;
    int m_countdownTime;
    bool m_isCountdownRunning;

    // System
    QTimer* m_timer;
    QSettings* m_settings;
};

#endif // TIKRA_HPP