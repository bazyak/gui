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
    title: qsTr('Lesson 09')

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
        var taskComp = task
                + 'title: "' + title
                + '"; deadline: "' + deadline
                + '"; progress: "' + progress + '" }'
        Qt.createQmlObject(taskComp, list, 'task' + count++)
        listHeader.updateCount()
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
    }

    ScrollView
    {
        id: scroller
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Component.onCompleted: loader.loadTasks()

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

                Text
                {
                    id: listHeader
                    anchors
                    {
                        bottom: parent.bottom
                        bottomMargin: 10
                        left: parent.left
                        leftMargin: 10
                    }
                    font.bold: true
                    text: qsTr('All tasks')

                    function updateCount()
                    {
                        text = qsTr('All tasks - ')
                            + ((root.count + loader.loadedTasks)
                            + ' /loaded from file - ' + loader.loadedTasks
                            + ', new - ' + root.count + '/')
                    }
                }
            }
        }
    }

    Connections
    {
        target: Qt.application

        function onAboutToQuit() { loader.saveTasks() }
    }
}

