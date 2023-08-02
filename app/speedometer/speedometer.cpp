#include <QPainter>
#include "speedometer.h"

Speedometer::Speedometer(QQuickItem *parent)
    :QQuickPaintedItem(parent),
      m_SpeedometerSize(500),                   // depends on screen size
      m_StartAngle(50),                         // needle resting position
      m_AlignAngle(260),                        // should be 360-m_StartAngle*3 for good looking
      m_LowestRange(0),                         //
      m_HighestRange(4000),                     //
      m_Speed(0),                              // speed value
      m_ArcWidth(10),                           // thighness /width of outer arc
      m_OuterColor(QColor("#375b7c")),          // outer arc
      m_InnerColor(QColor("white")),            // inner pie
      m_TextColor(QColor("black")),             // text tat displays the speed
    m_BackgroundColor(Qt::transparent)          //
{

}

//virtual paint method (here the speedometer is drawn on the qml canvas)
void Speedometer::paint(QPainter *painter)
{
    QRectF rect = this->boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();                      // Pen to draw
    pen.setCapStyle(Qt::FlatCap);

    double startAngle;
    double spanAngle;

    startAngle = m_StartAngle - 40;
    spanAngle = 0 - m_AlignAngle;

    //form: outer arc (static)
    painter->save();
    pen.setWidth(m_ArcWidth);
    pen.setColor(m_InnerColor);
    painter->setPen(pen);
    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, spanAngle * 16);
    painter->restore();

    //form: inner pie (static)
    int pieSize = m_SpeedometerSize/5;
    painter->save();
    pen.setWidth(m_ArcWidth/2);
    pen.setColor(m_OuterColor);
    painter->setBrush(m_InnerColor);
    painter->setPen(pen);
    painter->drawPie(rect.adjusted(pieSize, pieSize, -pieSize, -pieSize), startAngle * 16, spanAngle * 16);
    painter->restore();

    //shows the speed value in inner pie
    painter->save();
    QFont font("Arial",42,QFont::Bold);
    painter->setFont(font);
    pen.setColor(m_TextColor);
    painter->setPen(pen);
    painter->drawText(rect.adjusted(m_SpeedometerSize/30, m_SpeedometerSize/30, -m_SpeedometerSize/30, -m_SpeedometerSize/4), Qt::AlignCenter  ,QString::number((m_Speed/40),'f',1));
    font.setPointSize(15);
    painter->setFont(font);
    painter->drawText(rect.adjusted(m_SpeedometerSize/30, m_SpeedometerSize/30, -m_SpeedometerSize/30, -m_SpeedometerSize/10), Qt::AlignCenter  ,QString::fromStdString("km/h"));
    painter->restore();

    //shows current speed in outer arc
    painter->save();
    pen.setWidth(m_ArcWidth);
    pen.setColor(m_OuterColor);
    qreal valueToAngle = ((m_Speed - m_LowestRange)/(m_HighestRange - m_LowestRange)) * spanAngle;
    painter->setPen(pen);
    painter->drawArc(rect.adjusted(m_ArcWidth, m_ArcWidth, -m_ArcWidth, -m_ArcWidth), startAngle * 16, valueToAngle * 16);
    painter->restore();

}

// getter
qreal   Speedometer::getSpeedometerSize()
{
    return m_SpeedometerSize;
}

qreal   Speedometer::getStartAngle()
{
    return m_StartAngle;
}

qreal   Speedometer::getAlignAngle()
{
    return m_AlignAngle;
}

qreal   Speedometer::getLowestRange()
{
    return m_LowestRange;
}

qreal   Speedometer::getHighestRange()
{
    return m_HighestRange;
}

qreal   Speedometer::getSpeed()
{
    return m_Speed;
}

int     Speedometer::getArcWidth()
{
    return m_ArcWidth;
}

QColor  Speedometer::getOuterColor()
{
    return m_OuterColor;
}

QColor  Speedometer::getInnerColor()
{
    return m_InnerColor;
}

QColor  Speedometer::getTextColor()
{
    return m_TextColor;
}

QColor  Speedometer::getBackgroundColor()
{
    return m_BackgroundColor;
}


// setter (the emit will effect the qml part)
void Speedometer::setSpeedometerSize(qreal size)
{
    if(m_SpeedometerSize == size)
        return;
    m_SpeedometerSize = size;

    emit speedometerSizeChanged();
}

void Speedometer::setStartAngle(qreal startAngle)
{
    if(m_StartAngle == startAngle)
        return;

    m_StartAngle = startAngle;

    emit startAngleChanged();
}

void Speedometer::setAlignAngle(qreal angle)
{
    if(m_StartAngle == angle)
        return;

    m_StartAngle = angle;

    emit alignAngleChanged();
}

void Speedometer::setLowestRange(qreal lowestRange)
{
    if(m_LowestRange == lowestRange)
        return;

    m_LowestRange = lowestRange;

    emit lowestRangeChanged();
}

void Speedometer::setHighestRange(qreal highestRange)
{
    if(m_HighestRange == highestRange)
        return;

    m_HighestRange = highestRange;

    emit highestRangeChanged();
}

void Speedometer::setSpeed(qreal speed)
{
    if(m_Speed == speed)
        return;

    m_Speed = speed;

    update();

    emit speedChanged();
}

void Speedometer::setArcWidth(int arcWidth)
{
    if(m_ArcWidth == arcWidth)
        return;

    m_ArcWidth = arcWidth;

    emit arcWidthChanged();
}

void Speedometer::setOuterColor(QColor outerColor)
{
    if(m_OuterColor == outerColor)
        return;

    m_OuterColor = outerColor;

    emit outerColorChanged();
}

void Speedometer::setInnerColor(QColor innerColor)
{
    if(m_InnerColor == innerColor)
        return;

    m_InnerColor = innerColor;

    emit innerColorChanged();
}

void Speedometer::setTextColor(QColor textColor)
{
    if(m_TextColor == textColor)
        return;

    m_TextColor = textColor;

    emit textColorChanged();
}

void Speedometer::setBackgroundColor(QColor backgroundColor)
{
    if(m_BackgroundColor == backgroundColor)
        return;

    m_BackgroundColor = backgroundColor;

    emit backgroundColorChanged();
}
