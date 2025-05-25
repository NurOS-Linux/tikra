#include "../include/tikra.hpp"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setOrganizationName("AetherDE");
    app.setOrganizationDomain("aetherde.org");
    app.setApplicationName("Tikra");
    app.setApplicationVersion("1.0.0");
    app.setApplicationDisplayName("Aether Tikra");
    
    // Create and show main window
    TikraMainWindow window;
    window.show();
    
    qDebug() << "Aether Tikra started successfully";
    
    return app.exec();
}