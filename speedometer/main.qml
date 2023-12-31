import QtQuick              2.9
import QtQuick.Window       2.2
import piracer.speedometer  1.0
import QtQuick.Controls     2.0
import QtQuick.Controls     1.2
import QtQuick.Layouts      1.1

// build the GUI here
Window {
    visible:    true
    width:      640
    height:     480
    title:      qsTr("Speedometer")
    color:      "light gray"

    Speedometer
    {
        objectName:                 "speedoMeter"
        anchors.horizontalCenter:   parent.horizontalCenter
        width:                      speedometerSize
        height:                     speedometerSize
        startAngle:                 startAngle
        alignAngle:                 alignAngle
        lowestRange:                lowestRange
        highestRange:               highestRange
        speed:                      speed
        arcWidth:                   arcWidth
        outerColor:                 outerColor
        innerColor:                 innerColor
        textColor:                  textColor
        backgroundColor:            backgroundColor
    }

}
