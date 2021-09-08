#include "QCalcWidget.h"
#include "ui_QCalcWidget.h"
#include <iostream>

/*!
 * \brief Sets up the UI and signal/slot connections for the calculator buttons.
 * \param parent Widget parent used for default widget behavior and widget cleanup.
 */
QCalcWidget::QCalcWidget( QWidget* parent ) : QWidget( parent ), vUi( new Ui::QCalcWidget )
{
  vUi->setupUi( this );

  resetLineEdit();

  auto defaultPalette = vUi->mHistory->palette();
  defaultPalette.setColor( QPalette::Base, defaultPalette.color( QPalette::Window ) );
  vUi->mHistory->setPalette( defaultPalette );
  vUi->mHistory->setFrame( false );

  std::map<QToolButton*, int> numberButtons{ { vUi->mButton_0, 0 }, { vUi->mButton_1, 1 },
                                             { vUi->mButton_2, 2 }, { vUi->mButton_3, 3 },
                                             { vUi->mButton_4, 4 }, { vUi->mButton_5, 5 },
                                             { vUi->mButton_6, 6 }, { vUi->mButton_7, 7 },
                                             { vUi->mButton_8, 8 }, { vUi->mButton_9, 9 } };

  for ( auto& button : numberButtons ) {
    connect( button.first, &QAbstractButton::pressed, this, [&, button]() {
      numberButtonPushed( button.second );
    } );
  }

  connect( vUi->mButton_Clear, &QAbstractButton::pressed, this, [&]() { clearButtonPushed(); } );

  std::vector<std::tuple<QToolButton*, Operations>> opsButtons{ { vUi->mButton_Plus, Plus },
                                                                { vUi->mButton_Multiply, Multiply },
                                                                { vUi->mButton_Divide, Divide },
                                                                { vUi->mButton_Subtract, Minus } };

  for ( auto& button : opsButtons ) {
    connect( std::get<0>( button ), &QAbstractButton::pressed, this, [&, button]() {
      operationButtonPushed( std::get<1>( button ) );
      vOperation = std::get<1>( button );
    } );
  }

  connect( vUi->mButton_Answer, &QAbstractButton::pressed, this, [&]() { equalsButtonPushed(); } );

  connect( vUi->mButton_Dot, &QAbstractButton::pressed, this, [&]() { addDecimalIfApplicable(); } );


  // Extra buttons to complete the grid that have not been implemented yet.
  vUi->mButton_LParen->hide();
  vUi->mButton_RParen->hide();
  vUi->mButton_Unknown->hide();
}

QCalcWidget::~QCalcWidget() = default;


/*!
 * \brief Called when any of the number buttons are pushed.
 *        Specialized logic for if this is the first time a number has been pushed after an
 *        operation or equals that overrides the number left in the line edit.
 * \param number The number on the corresponding button that was pushed.
 */
void QCalcWidget::numberButtonPushed( int number )
{
  if ( number == 0 && vUi->mLineEdit->text() == "0" ) {
    return;
  }
  if ( vUi->mLineEdit->text() == "0" || vFirst ) {
    vUi->mLineEdit->clear();
  }
  vUi->mLineEdit->setText( vUi->mLineEdit->text() + QString( "%1" ).arg( number ) );

  vFirst = false;
}

/*!
 * \brief Called when the clear button is pushed. Resets all private data and line edit displays
 *        back to their original state.
 */
void QCalcWidget::clearButtonPushed()
{
  resetLineEdit();
  vUi->mHistory->clear();
  vFirstNumber.reset();
  vSecondNumber.reset();
  vOperation = None;
  vFirst = true;
}


/*!
 * \brief Called when an operation button is pushed. If this is the first time an operation has
 *        been pressed, the number is stored into the "First Number" data. If it is the second
 *        (chaining operations together) the number is stored in the "Second Number" before
 *        performing the previous operation.
 * \param op The operation corresponding to the button that was pushed
 */
void QCalcWidget::operationButtonPushed( Operations op )
{
  if ( vFirstNumber ) {
    QString number = vUi->mLineEdit->text();
    vSecondNumber = number.toDouble();
    calculation();
  }
  QString number = vUi->mLineEdit->text();
  vFirstNumber = number.toDouble();
  vUi->mHistory->setText( number + " " + operationToString( op ) + " " );
  vSecondNumber.reset();
  vFirst = true;
}

/*!
 * \brief Hard code the ability to reset the input line to zero to avoid hardcoding the
 *        zero string in multiple locations.
 */
void QCalcWidget::resetLineEdit()
{
  vUi->mLineEdit->setText( "0" );
}


/*!
 * \brief Called when the equals button is pushed to do the mathematical calculations.
 *        Different behavior if the equals button is pressed twice in a row, it attempts
 *        to perform the calculation again with the previous result. (Behavior is similar to
 *        iphone calculator and Windows calculator)
 */
void QCalcWidget::equalsButtonPushed()
{
  QString number = vUi->mLineEdit->text();
  if ( vSecondNumber ) {
    vFirstNumber = number.toDouble();
  } else {
    vSecondNumber = number.toDouble();
  }

  vUi->mHistory->setText( QString( "%1" ).arg( *vFirstNumber ) + " " +
                          operationToString( vOperation ) + " " +
                          QString( "%1" ).arg( *vSecondNumber ) + " = " );

  calculation();
  vFirst = true;
  vFirstNumber.reset();
}


/*!
 * \brief Hardcoded string representations of the valid calculator operations.
 * \param op The given operation
 * \return The string representation
 */
QString QCalcWidget::operationToString( Operations op )
{
  switch ( op ) {
    case Plus:
      return "+";
    case Minus:
      return "−";
    case Multiply:
      return "×";
    case Divide:
      return "÷";
    case None:
      break;
  }
  return "";
}


/*!
 * \brief Called when the decimal button it pressed. This prevents the decimal from being added
 *        to a number more than once.
 */
void QCalcWidget::addDecimalIfApplicable()
{
  if ( vFirst ) {
    vUi->mLineEdit->setText( "0." );
    vFirst = false;
    return;
  }
  QString currentNumber = vUi->mLineEdit->text();
  if ( currentNumber.contains( "." ) ) {
    return;
  }
  vUi->mLineEdit->setText( currentNumber + "." );
}


/*!
 * \brief Performs the mathmatical calculation for the given values and the operation.
 *        This function is only valid for single operation with two numbers and one operand.
 */
void QCalcWidget::calculation()
{
  double answer = 0;
  switch ( vOperation ) {
    case Plus:
      answer = *vFirstNumber + *vSecondNumber;
      break;
    case Minus:
      answer = *vFirstNumber - *vSecondNumber;
      break;
    case Multiply:
      answer = *vFirstNumber * *vSecondNumber;
      break;
    case Divide:
      answer = *vFirstNumber / *vSecondNumber;
      break;
    case None:
      break;
  }

  vUi->mLineEdit->setText( QString( "%1" ).arg( answer ) );
}
