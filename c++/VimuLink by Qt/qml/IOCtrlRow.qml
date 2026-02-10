// IOCtrlRow.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: root
    property int ioIndex: 0
    property var ioModel: vmusbwave
    spacing: 4
    Layout.fillWidth: true

    CheckBox {
        id: ctrlEn
        text: "IO" + root.ioIndex
        Layout.preferredWidth: 90
        checked: root.ioModel.getIOEnable(root.ioIndex)
        onClicked: root.ioModel.setIOEnable(root.ioIndex, checked)
    }

    ComboBox {
        id: inOutCombo
        Layout.preferredWidth: 90
        model: ["Input", "Output"]
        currentIndex: root.ioModel.getIOInOut(root.ioIndex)
        onActivated: root.ioModel.setIOInOut(root.ioIndex, currentIndex)
    }

    ComboBox {
        id: outStateCombo
        Layout.preferredWidth: 130
        enabled: inOutCombo.currentIndex === 1
        model: ["0", "1", "Z", "Freq", "Dds Gate"]
        currentIndex: root.ioModel.getIOOutState(root.ioIndex)
        onActivated: root.ioModel.setIOOutState(root.ioIndex, currentIndex)
    }

    Rectangle {
        id: inStateIndicator
        Layout.preferredWidth: 30
        height: 30
        color: root.ioModel.getIOInState(root.ioIndex)? "red" : "green"
        border.width: 1
        border.color: "#ccc"

        // 可以添加动画效果
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    TextField {
        id: freqEdit
        Layout.preferredWidth: 150
        enabled: inOutCombo.currentIndex === 1 && outStateCombo.currentIndex === 3
        placeholderText: "Freq(Hz)"
        validator: DoubleValidator {
            bottom: 0.1
            top: 10000000
        }
        text: root.ioModel.getIOFreq(root.ioIndex)
        onEditingFinished: {
            if (acceptableInput)
                root.ioModel.setIOFreq(root.ioIndex, Number(text))
        }
    }

    TextField {
        id: dutyEdit
        Layout.preferredWidth: 150
        enabled: inOutCombo.currentIndex === 1 && outStateCombo.currentIndex === 3
        placeholderText: "Duty(%)"
        validator: DoubleValidator {
            bottom: 0.1
            top: 99.9
        }
        text: root.ioModel.getIODuty(root.ioIndex)
        onEditingFinished: {
            if (acceptableInput)
                root.ioModel.setIODuty(root.ioIndex, Number(text))
        }
    }

    // 监听模型变化
    Connections {
        target: root.ioModel
        function onIoStateChanged(index) {
            //console.log("onIoStateChanged " + index);
            if (index === root.ioIndex || index === -1) {
                // 更新UI
                ctrlEn.checked = root.ioModel.getIOEnable(root.ioIndex)
                inOutCombo.currentIndex = root.ioModel.getIOInOut(root.ioIndex)
                outStateCombo.currentIndex = root.ioModel.getIOOutState(root.ioIndex)
                freqEdit.text = root.ioModel.getIOFreq(root.ioIndex)
                dutyEdit.text = root.ioModel.getIODuty(root.ioIndex)
                inStateIndicator.color = root.ioModel.getIOInState(root.ioIndex)? "red" : "green"
            }
        }
    }

    Connections {
        target: root.ioModel
        function onIoInStateChanged(index) {
            //console.log("onIoInStateChanged " + index);
            if (index === root.ioIndex || index === -1) {
                inStateIndicator.color = root.ioModel.getIOInState(root.ioIndex)? "red" : "green"
            }
        }
    }
}
