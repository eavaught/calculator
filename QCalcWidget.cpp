#include "QCalcWidget.h"
#include "ui_QCalcWidget.h"
#include <iostream>


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

  connect( vUi->mButton_Answer, &QAbstractButton::pressed, this, [&]() { calculateAnswer(); } );

  connect( vUi->mButton_Dot, &QAbstractButton::pressed, this, [&]() { addDecimalIfApplicable(); } );
}

QCalcWidget::~QCalcWidget() = default;


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

void QCalcWidget::clearButtonPushed()
{
  resetLineEdit();
  vUi->mHistory->clear();
  vFirstNumber.reset();
  vSecondNumber.reset();
  vOperation = None;
  vFirst = true;
}

void QCalcWidget::operationButtonPushed( Operations op )
{
  if ( vFirstNumber ) {
    QString number = vUi->mLineEdit->text();
    vSecondNumber = number.toDouble();
    math();
  }
  QString number = vUi->mLineEdit->text();
  vFirstNumber = number.toDouble();
  vUi->mHistory->setText( number + " " + operationToString( op ) + " " );
  vSecondNumber.reset();
  vFirst = true;
}

void QCalcWidget::resetLineEdit()
{
  vUi->mLineEdit->setText( "0" );
}

void QCalcWidget::calculateAnswer()
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

  math();
  vFirst = true;
}


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


void QCalcWidget::addDecimalIfApplicable()
{
  QString currentNumber = vUi->mLineEdit->text();
  if ( currentNumber.contains( "." ) ) {
    return;
  }
  vUi->mLineEdit->setText( currentNumber + "." );
}

void QCalcWidget::math()
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
