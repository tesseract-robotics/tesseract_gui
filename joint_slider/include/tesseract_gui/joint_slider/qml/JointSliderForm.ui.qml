import QtQml.Models 2.2
import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: jointSliderElement

    property alias jointSliderListView: jointSliderListView

    ListView {
        id: jointSliderListView
        anchors.fill: parent
        spacing: 5

        model: jointSliderModel

        ScrollIndicator.vertical: ScrollIndicator {}
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
