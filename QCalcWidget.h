#pragma once

#include <QWidget>
#include <optional>


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
  void operationButtonPushed( Operations op );
  void resetLineEdit();
  void calculateAnswer();
  QString operationToString( Operations op );
  void addDecimalIfApplicable();
  void math();

private:
  std::unique_ptr<Ui::QCalcWidget> vUi;

  std::optional<double> vFirstNumber;
  std::optional<double> vSecondNumber;
  Operations vOperation = None;

  bool vFirst = true;
};
