import QtQuick
import QtQuick.Controls

Button
{
    property string frontColor: 'plum'
    property string frontColorHovered: 'deeppink'
    property string frontColorPressed: 'deeppink'
    property string backColor: 'lightcyan'

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
        implicitHeight: parent.height
        opacity: enabled ? 1 : 0.3
        border.color: control.down || control.hovered ? frontColorHovered : frontColor
        border.width: 1
        color: backColor
        radius: 5
    }
}
