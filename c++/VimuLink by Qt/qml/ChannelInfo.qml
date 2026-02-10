import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Controls.Universal 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle  {
    property string channelName: "CHX"
    property color primaryColor: "blue"
    property color backColor: "white"
    //property color accentColor: "red"
    property color channelColor: "gray"

    property int buttonImplicitHeight: 36

    //anchors.fill: parent
    //border.width: 2
    radius: 4
    //border.color: channelColor
    color: backColor

    RowLayout {
        anchors.fill: parent
        //anchors.margins: 2

        Button {
            id: ctrl_btn_dec
            Layout.preferredWidth: 24
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter // 垂直居中
            //checked: !hostCtrl.isPause
            text: qsTr("-")
            /*background:
                Rectangle  {
                    radius: 4
                    anchors.fill: parent
                    color: ctrl_btn_dec.down? backColor : primaryColor
                    border.color: ctrl_btn_dec.focus||ctrl_btn_dec.hovered? accentColor : backColor
            }*/
            onClicked: console.log("ctrl_btn_dec onClicked");
        }

        ColumnLayout {
            height: ctrl_btn_dec.height
            Layout.fillWidth: true

            Rectangle  {
                    height: 2
                    Layout.fillWidth: true
                    color: channelColor
            }

            Rectangle {
                anchors.margins: 2
                color: backColor
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout{
                    anchors.fill: parent

                    ColumnLayout{
                        implicitWidth: parent.width/2
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignLeft

                        /*Rectangle  {
                                anchors.fill: parent
                                color: channelColor
                        }*/

                        CheckBox {
                            implicitHeight: buttonImplicitHeight
                            //implicitWidth: parent.width
                            //implicitHeight: parent.height/2
                            Layout.topMargin: 6  // 上方外部空隙
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignTop|Qt.AlignLeft
                            text: qsTr("CH1")
                        }

                        ComboBox {
                            implicitHeight: buttonImplicitHeight
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignBottom|Qt.AlignLeft
                            //Layout.topMargin: 10  // 上方外部空隙
                            Layout.bottomMargin: 6 // 下方外部空隙
                            Layout.leftMargin: 4  // 左侧外部空隙
                            //Layout.rightMargin: 10 // 右侧外部空隙
                            //text: qsTr("1V")
                            model: ["1V","2V","5V"]
                            currentIndex: 0
                        }
                    }
                }
            }
        }

        Button {
            id: ctrl_btn_add
            Layout.preferredWidth: 24
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter // 垂直居中
            //checked: !hostCtrl.isPause
            text: qsTr("+")
            /*background:
                Rectangle  {
                    radius: 4
                    anchors.fill: parent
                    color: ctrl_btn_add.down? backColor : primaryColor
                    border.color: ctrl_btn_add.focus||ctrl_btn_add.hovered? "red" : backColor
            }*/
            onClicked: console.log("ctrl_btn_add onClicked");
        }

    }
}
