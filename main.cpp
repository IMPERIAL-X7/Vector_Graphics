#include <QApplication>
// #include <QMainWindow>
// #include "core/Diagram.h"
// #include "shapes/Circle.h"
// #include "io/SVGWriter.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;
  // window.setWindowTitle("Vector Graphics Editor");
  // window.resize(800, 600);
  window.show();

  return app.exec();
}
