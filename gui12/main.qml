import QtQuick
import QtQml
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import com.bazyak.SearchService

Window
{
    id: root
    visible: true
    minimumWidth: 470
    minimumHeight: 550
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    title: qsTr('Lesson 12')

    readonly property int kWidth: 415
    readonly property int kToFindHeight: 50
    readonly property int kToFindLength: 40
    readonly property int kCompsHeight: 25
    readonly property int kSpacing: 10
    readonly property string kPicture: 'Picture { imageWidth: ' + kWidth + '; url: '
    readonly property string kErrorTitle: qsTr('Something went wrong')
    readonly property string kPlaceholderText: qsTr('Let\'s find something')
    readonly property string kButtonText: qsTr('Search')
    readonly property string kPopupColor: '#1C2833'

    function searchPictures()
    {
        if (to_find.length)
        {
            engine.searchPictures(to_find.text)
        }
    }

    SearchService
    {
        id: engine

        onSearchDone: (succeed, error) =>
                      {
                          for (var i = list.children.length; i > 1; --i)
                          {
                              list.children[i-1].destroy()
                          }
                          if (!succeed)
                          {
                              errorTitle.text = qsTr(kErrorTitle)
                              errorBody.text = qsTr(error)
                              popup.open()
                              return
                          }
                      }

        onItemFound: (item) =>
                     {
                         var pictureComp = kPicture + '"http:' + item + '" }'
                         Qt.createQmlObject(pictureComp, list, 'picture')
                     }
    }

    Popup
    {
        id: popup
        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: kWidth * 0.95
        height: kWidth * 0.3
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        onClosed: to_find.forceActiveFocus()

        background: Rectangle
        {
            anchors.fill: parent
            border.color: kPopupColor
            color: kPopupColor
            radius: 7
        }

        contentItem: Column
        {
            Text
            {
                id: errorTitle
                color: 'white'
                font.bold: true
                anchors.top: parent.top
                anchors.topMargin: 5
                height: kCompsHeight
                horizontalAlignment: Qt.AlignCenter
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text
            {
                id: errorBody
                color: 'white'
                anchors.top: errorTitle.bottom
                width: parent.width
                horizontalAlignment: Qt.AlignCenter
                wrapMode: Text.WordWrap
            }
        }
    }

    ScrollView
    {
        id: scroller
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Column
        {
            id: list
            spacing: kSpacing
            width: root.width
            height: children.height
            bottomPadding: kSpacing

            Row
            {
                width: kWidth
                height: root.kToFindHeight
                anchors.horizontalCenter: parent.horizontalCenter

                TextField
                {
                    id: to_find
                    placeholderText: kPlaceholderText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    width: kWidth * 0.7
                    height: kCompsHeight
                    maximumLength: root.kToFindLength
                    selectByMouse: true
                    wrapMode: TextEdit.Wrap

                    Component.onCompleted: forceActiveFocus()
                    Keys.onReturnPressed: searchPictures()
                }

                ModernButton
                {
                    id: search
                    text: kButtonText

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    width: kWidth * 0.25
                    height: kCompsHeight

                    onClicked: searchPictures()
                }
            }
        }
    }
}

