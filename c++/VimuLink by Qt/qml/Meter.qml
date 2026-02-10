import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: meterdlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property var usbModel: vmusbwave

    Connections {
        target: meterdlg.usbModel
        //function onSampleIndexChanged() {
        //    console.log("meterdlg sampleIndexChanged " + powerdlg.usbModel.getSample());
        //}
    }

    property bool meterOpen: false
    onOpened: meterOpen = true;
    onClosed: meterOpen = false;
    function switchDlg()
    {
        if(meterOpen) {
            meterdlg.close()
            meterOpen = false;
        }
        else {
            meterdlg.open()
            meterOpen = true;
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            ColumnLayout {
                Layout.fillWidth: true

                RowLayout {
                    Layout.fillWidth: true

                    Item {
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: meterEn
                        enabled: usbModel.meterSupport
                        text: "Meter"
                        focusPolicy: Qt.TabFocus

                        onClicked: {
                            usbModel.setMeterEn(checked)
                            if(checked)
                                usbModel.setMeterMode(0x000001)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    // 添加边框
                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "transparent"  // 透明背景
                        border.color: "gray"
                        border.width: 1
                        radius: 2  // 可选：圆角
                    }

                    Item {
                        Layout.preferredWidth: 50
                    }

                    Label {
                        Layout.fillWidth: true
                        height: textTop.height+textBottom.height
                        Layout.alignment: Qt.AlignCenter
                        text: usbModel.meterText
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 50

                        Label {
                            id:textTop

                            text: usbModel.meterTextTop
                        }

                        Label {
                            id:textBottom
                            text: usbModel.meterTextBottom
                        }
                    }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 3  // 指定列数
                    rowSpacing: 5
                    columnSpacing: 5

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "V(DC)"

                        onClicked: usbModel.setMeterMode(0x000001) //METER_DC_V
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "V(AC)"

                        onClicked: usbModel.setMeterMode(0x000002)  //METER_AC_V
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "Ω(2线)"

                        onClicked: usbModel.setMeterMode(0x000800) //METER_OM2
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "A(DC)"

                        onClicked: usbModel.setMeterMode(0x000008) //METER_DC_A
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "A(AC)"

                        onClicked: usbModel.setMeterMode(0x000040)  //METER_AC_A
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "Ω(4线)"

                        onClicked: usbModel.setMeterMode(0x001000)  //METER_OM4
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "mA(DC)"

                        onClicked: usbModel.setMeterMode(0x000010)  //METER_DC_MA
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "mA(AC)"

                        onClicked: usbModel.setMeterMode(0x000080)  //METER_AC_MA
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "Diode"

                        onClicked: usbModel.setMeterMode(0x010000)  //METER_DIODE
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "uA(DC)"

                        onClicked: usbModel.setMeterMode(0x000020)  //METER_DC_UA
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "0Ω"

                        onClicked: usbModel.setMeterMode(0x020000)  //METER_0OM
                    }

                    Button {
                        Layout.fillWidth: true  // 填充宽度
                        Layout.preferredHeight: 50
                        text: "Zero"

                        onClicked: usbModel.setMeterMode(0x100000)  //METER_ZERO
                    }
                }
            }


        }
    }
}
