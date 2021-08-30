#pragma once

#include <QWidget>

namespace Ui {
class QCalcWidget;
}

class QCalcWidget : public QWidget
{
  Q_OBJECT
public:
  explicit QCalcWidget( QWidget* parent = nullptr );
  ~QCalcWidget() override;

private:
  enum Operations { Plus, Minus, Multiply, Divide, None };

private:
  void numberButtonPushed( int number );
  void clearButtonPushed();
  void moveTextToHistory( QString ops );
  void resetLineEdit();
  void calculateAnswer();

private:
  std::unique_ptr<Ui::QCalcWidget> vUi;

  int firstNumber = 0;
  int secondNumber = 0;
  Operations operation = None;
};
