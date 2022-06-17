import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle
{
    property int imageWidth: 415
    property string url: ''
    property string borderColor: 'whitesmoke'

    width: kWidth
    height: kWidth
    anchors.horizontalCenter: parent.horizontalCenter
    border.color: borderColor

    Image
    {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: url
    }

    BusyIndicator
    {
        anchors.centerIn: parent
        running: image.status === Image.Loading
    }
}
