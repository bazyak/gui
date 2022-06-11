import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle
{
    readonly property int kWidth: 415
    readonly property int kHeight: 120

    property string title: ''
    property string deadline: ''
    property string progress: ''
    property int bottomMargin: 0

    id: comps
    color: 'paleturquoise'
    width: kWidth
    height: kHeight
    radius: 7
    Layout.alignment: Qt.AlignCenter
    Layout.bottomMargin: bottomMargin

    Text
    {
        anchors
        {
            top: parent.top
            topMargin: 15
            left: parent.left
            leftMargin: 10
        }
        text: qsTr('Task')
    }

    Text
    {
        text: title
        anchors
        {
            top: parent.top
            topMargin: 15
            left: parent.left
            leftMargin: 100
        }
    }

    Text
    {
        anchors
        {
            top: parent.top
            topMargin: 50
            left: parent.left
            leftMargin: 10
        }
        text: qsTr('Deadline')
    }

    Text
    {
        text: deadline
        anchors
        {
            top: parent.top
            topMargin: 50
            left: parent.left
            leftMargin: 100
        }
    }

    Text
    {
        anchors
        {
            top: parent.top
            topMargin: 85
            left: parent.left
            leftMargin: 10
        }
        text: qsTr('Progress')
    }

    ProgressBar
    {
        id: control
        value: parseFloat(progress) / 10

        anchors
        {
            top: parent.top
            topMargin: 92
            left: parent.left
            leftMargin: 100
        }

        background: Rectangle
        {
            implicitWidth: 300
            implicitHeight: 5
            color: 'lightgray'
            radius: 3
        }

        contentItem: Item
        {
            implicitWidth: 300
            implicitHeight: 5

            Rectangle
            {
                width: control.visualPosition * parent.width
                height: parent.height
                radius: 3
                color: '#17a81a'
            }
        }
    }
}
