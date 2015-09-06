import QtQuick 2.3
import QtQuick.Window 2.2
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Item {
    id: mainWindow
    visible: true
    width: 500; height: 500

    Chess.BoardView {
        id: view
            // Chess.BoardModel
            // {
            //     id: model
            // }
    }

    TestCase {

    }
}


