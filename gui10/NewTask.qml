import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Rectangle
{
    readonly property int kWidth: 415
    readonly property int kHeight: 120

    id: mainrect
    width: kWidth
    height: kHeight
    z: 1000
    Layout.alignment: Qt.AlignCenter

    Text
    {
        anchors
        {
            top: parent.top
            topMargin: 7
            left: parent.left
            leftMargin: 10
        }
        text: qsTr('Add new task')
        font.bold: true
    }

    Rectangle
    {
        anchors.top: parent.top
        anchors.topMargin: 30
        width: parent.width
        height: parent.height - 30
        radius: 7
        color: 'lavenderblush'

        Text
        {
            anchors
            {
                top: parent.top
                topMargin: 15
                left: parent.left
                leftMargin: 10
            }
            text: qsTr('Title')
        }

        TextField
        {
            id: title
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 100
            width: 300
            height: 25
            maximumLength: 50
            selectByMouse: true
            wrapMode: TextEdit.Wrap

            Component.onCompleted: forceActiveFocus()
            Keys.onReturnPressed: deadline.focus = true
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

        TextField
        {
            id: deadline
            anchors.top: parent.top
            anchors.topMargin: 45
            anchors.left: parent.left
            anchors.leftMargin: 100
            width: 300
            height: 25
            inputMask: '99.99.9999;_'
            selectByMouse: true

            onPressed: datePicker.visible = !datePicker.visible
            Keys.onReturnPressed: datePicker.visible = false
        }

        DatePicker
        {
            id: datePicker
            visible: false
            width: 300
            height: 200
            z: 1000
            anchors
            {
                left: parent.left
                leftMargin: 100
                top: parent.top
                topMargin: 75
            }

            Component.onCompleted: set(new Date()) // today
            onClicked: date =>
            {
                visible = false
                deadline.text = Qt.formatDate(date, 'dd.MM.yyyy')
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

        Slider
        {
            id: progress
            anchors
            {
                top: parent.top
                topMargin: 81
                left: parent.left
                leftMargin: 100
            }
            width: 300
            from: 0
            value: 3
            to: 10
            stepSize: 1.0
            snapMode: Slider.SnapAlways
        }

        Popup
        {
            id: popup
            parent: Overlay.overlay
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 400
            height: 100
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            background: Rectangle
            {
                anchors.fill: parent
                border.color: '#1C2833'
                color: '#1C2833'
                radius: 7
            }

            Text
            {
                id: error
                color: 'white'
                anchors.centerIn: parent
            }
        }

        ModernButton
        {
            id: addData
            text: qsTr('Add')
            frontColor: 'plum'
            frontColorPressed: 'deeppink'
            backColor: 'lightcyan'

            onClicked:
            {
                if (title.text.trim() === '' || deadline.text.trim() === '..')
                {
                    error.text = qsTr('Title or date cann\'t be empty')
                    popup.open()
                    return
                }
                mainrect.parent.addTask(title.text, deadline.text, progress.value + '')
            }

            Component.onCompleted:
            {
                anchors.right = title.right
                anchors.bottom = parent.bottom
                anchors.bottomMargin = 10
                width = 100
                height = 25
            }
        }
    }
}
