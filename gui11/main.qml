import QtQuick
import QtQml
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import com.bazyak.TaskService

Window
{
    id: root
    visible: true
    minimumWidth: 470
    minimumHeight: 480
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    title: qsTr('Lesson 11')

    readonly property int kWidth: 415
    readonly property int kTaskHeight: 120
    readonly property int kNewTaskHeight: 180
    readonly property int kListHeaderHeight: 40
    readonly property int kSpacing: 0
    readonly property int kTaskBottomMargin: 10
    readonly property string newTask: 'NewTask { width: ' + kWidth + '; height: ' + kNewTaskHeight + ' }'
    property string task: 'Task { width: ' + kWidth + '; height: ' + kTaskHeight
                          + '; bottomMargin: ' + kTaskBottomMargin + '; '
    property int count: 0

    function preSaveTask(title, deadline, progress)
    {
        loader.preSaveTask(title, deadline, progress)
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

    TaskService
    {
        id: loader
        property int loadedTasks: 0

        onInitEnd: (succeed, count) =>
        {
            if (succeed)
            {
                Qt.createQmlObject(newTask, head, 'new_task')
                loadedTasks = count
                listHeader.updateCount()
            }
            else Qt.quit()
        }

        onTaskSaved: (saved, title, deadline, progress) =>
        {
            if (!saved)
            {
                 error.text = qsTr('Task "' + title + '" already exists')
                 popup.open()
                 return
            }

            var taskComp = task
                    + 'title: "' + title
                    + '"; deadline: "' + deadline
                    + '"; progress: "' + progress + '" }'
            Qt.createQmlObject(taskComp, list, 'task' + count++)
            listHeader.updateCount()
        }
    }

    ScrollView
    {
        id: scroller
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Component.onCompleted: loader.loadBase()

        ColumnLayout
        {
            id: list
            spacing: kSpacing
            width: root.width
            height: children.height
            Layout.bottomMargin: 100

            Rectangle
            {
                id: head
                width: root.kWidth
                height: root.kNewTaskHeight
                z: 1000
                Layout.alignment: Qt.AlignCenter

                function addTask(title, deadline, progress)
                {
                    root.preSaveTask(title, deadline, progress)
                }
            }

            Rectangle
            {
                width: root.kWidth
                height: kListHeaderHeight
                Layout.alignment: Qt.AlignCenter

                RowLayout
                {
                    width: parent.width
                    height: parent.height

                    Text
                    {
                        id: listHeader
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        Layout.leftMargin: 10
                        font.bold: true
                        text: qsTr('All tasks')

                        function updateCount()
                        {
                            text = qsTr('All tasks - ')
                                + ((root.count + loader.loadedTasks)
                                + ' /loaded from db - ' + loader.loadedTasks
                                + ', new - ' + root.count + '/')
                        }
                    }

                    ModernButton
                    {
                        id: viewAll
                        text: qsTr('View all tasks')
                        frontColor: 'plum'
                        frontColorHovered: 'deeppink'
                        frontColorPressed: 'deeppink'
                        backColor: 'lightcyan'
                        width: 100
                        height: 25
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 15

                        onClicked:
                        {
                            loader.showAll()
                        }
                    }
                }
            }
        }
    }
}

