import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id:ioDlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property bool ioOpen: false
    onOpened: ioOpen = true;
    onClosed: ioOpen = false;
    function switchDlg()
    {
        if(ioOpen) {
            ioDlg.close()
            ioOpen = false;
        }
        else {
            ioDlg.open()
            ioOpen = true;
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ColumnLayout {
            spacing: 4
            width: parent.width

            // 使用Repeater动态生成IO控制行
            Repeater {
                model: vmusbwave.dIOChannelNum

                IOCtrlRow {
                    Layout.fillWidth: true
                    ioIndex: index
                    ioModel: vmusbwave
                }
            }

            RowLayout{
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: "PulseSyn"
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }
    }
}
