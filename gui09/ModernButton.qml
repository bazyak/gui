import QtQuick
import QtQuick.Controls

Button
{
    property string frontColor: ''
    property string frontColorPressed: ''
    property string backColor: ''

    id: control
    text: qsTr('ModernButton')
    height: 25
    width: 100

    contentItem: Text
    {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? frontColorPressed : frontColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle
    {
        implicitWidth: parent.width
        implicitHeight: parent.width
        opacity: enabled ? 1 : 0.3
        border.color: control.down ? frontColorPressed : frontColor
        border.width: 1
        color: backColor
        radius: 5
    }
}
