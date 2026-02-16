#include <QApplication>
// #include <QMainWindow>
// #include "core/Diagram.h"
// #include "shapes/Circle.h"
// #include "io/SVGWriter.h"
#include "ui/MainWindow.h"

// Application Entry Point: Initializes the Qt Loop and Main Window
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;
  window.show();

  return app.exec();
}
