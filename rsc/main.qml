import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Qt.labs.platform 1.1

Window {
    id: main_window
    title: "Face Folder Detector"
    visible: true
    width: 640
    height: 500
    maximumHeight: main_window.height
    maximumWidth: main_window.width
    minimumHeight: main_window.height
    minimumWidth:  main_window.width

    opacity: 0.8
    Material.theme: Material.LightBlue
    Material.accent: Material.Blue

    property color text_color: "#9fc8fd"


    SwipeView {
        id: view
        opacity: 0.896
        width: main_window.width
        height: main_window.height
        currentIndex: _backend.m_current_page
        //currentIndex: 2
        interactive: false


        Page {
            id: page_start
            title: qsTr("Home")

            GridLayout {
                id: gridLayout
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                columns: 1
                rows: 5

                ColumnLayout {
                    id: columnLayout
                    Layout.preferredWidth: page_start.width * 0.5
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    Label {
                        id: label
                        width: parent.width*0.9
                        color: text_color
                        text: qsTr("OPTIONS")
                        font.letterSpacing: 5
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                        font.italic: false
                        Layout.columnSpan: 0
                        font.wordSpacing: 2
                        anchors.rightMargin: 10
                        anchors.topMargin: 10
                        Layout.rowSpan: 1
                        Layout.fillHeight: true
                        font.styleName: "Полужирный"
                        anchors.leftMargin: 10
                        font.family: "Times New Roman"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        font.bold: true
                        Layout.margins: 10
                    }

                    Button {
                        id: button
                        text: qsTr("Select directory")
                        font.family: "Times New Roman"
                        highlighted: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.rowSpan: 5
                        Layout.margins: 5
                        Layout.fillWidth: true
                        onClicked: dlg_folder.open()
                        padding: 0
                    }

                    Label {
                        id: label1
                        height: 13
                        text: qsTr("FACE DETECTORS")
                        color: text_color
                        font.letterSpacing: 2
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.italic: false
                        font.wordSpacing: 1
                        anchors.rightMargin: 10
                        anchors.topMargin: 10
                        font.styleName: "Полужирный"
                        anchors.leftMargin: 10
                        font.family: "Times New Roman"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        font.bold: true
                        Layout.rowSpan: 3
                        Layout.margins: 5
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.columnSpan: 0
                    }

                    SpinBox {
                        font.family: "Times New Roman"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.rowSpan: 4
                        Layout.margins: 5
                        Layout.fillWidth: true
                        padding: 0
                        Layout.fillHeight: false
                        editable: false
                        to: 5
                        from: 1
                        value: _backend.m_count_handlers
                        onValueChanged: _backend.m_count_handlers = value
                    }

                    Button {
                        id: button1
                        text: qsTr("Search image with faces")
                        font.family: "Times New Roman"
                        highlighted: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.rowSpan: 5
                        Layout.margins: 5
                        Layout.fillWidth: true
                        padding: 0
                        onClicked: _backend.start_search()
                    }
                }
            }

        }



        Page {
            id: page_status
            title: qsTr("Discover")

            GridLayout {
                rows: 3
                columns: 1
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                RowLayout {
                    width: 100
                    height: 100
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillHeight: false
                    Layout.fillWidth: false

                    BusyIndicator {
                        id: busyIndicator
                        width: 100
                        Layout.preferredWidth: -1
                        Layout.fillHeight: true
                        Layout.preferredHeight: 100
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.maximumWidth: 65535
                        Layout.maximumHeight: 65535
                    }
                }

                RowLayout {
                    Label {
                        width: parent.width * 0.3
                        height: parent.height * 0.3
                        color: text_color
                        text: qsTr(_backend.m_status)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        layer.smooth: false
                        layer.textureMirroring: ShaderEffectSource.MirrorVertically
                        Layout.topMargin: 20
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                    }

                }
            }
        }

        Page {
            id: page_result
            title: qsTr("Discover")

            Rectangle{
                id: rectangle
                color: "#00000000"
                anchors.fill: parent
                /* ------------------- */

                ColumnLayout {
                    id: rowLayout
                    anchors.fill: parent
                    spacing: 1

                    ListView {
                        id: result_list
                        width: 300
                        height: 300
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        delegate: delegateItem
                        model: _backend.m_file_model // the C++ model is set here

                    }

                    Button {
                        id: button2
                        width: 300
                        text: qsTr("Go Back")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.fillHeight: false
                        Layout.fillWidth: false
                        highlighted: true
                        onClicked: _backend.m_current_page = 0
                    }
                }

                Component {
                    id: delegateItem
                    Item {
                        height: 150

                        Row {
                            id: column
                            spacing: 5
                            Image {
                                width: 80
                                height: 80
                                anchors.verticalCenter: parent.verticalCenter
                                source: "file:///" + model.modelData
                                asynchronous: true
                            }
                            Text {
                                id: name
                                color: text_color
                                text: model.modelData
                                anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }


                // end delegateItem
            } //end Rectangle

        }


    }


    FolderDialog  {
        id: dlg_folder
        title: "Please choose a folder"
        visible: false
        onAccepted: {

            var path = dlg_folder.currentFolder.toString();
            path = path.replace(/^(file:\/{3})/,"");
            var cleanPath = decodeURIComponent(path);
            console.log(cleanPath)
            _backend.open_dialog(cleanPath)

            dlg_folder.close()
        }
        onRejected: {
            dlg_folder.close()
        }
    }

}












