import QtQuick 2.3
import QtQuick.Window 2.2
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Rectangle {

    Chess.BoardModel
    {
        id: board
    }

    TestCase {

        function init()
        {
            board.clearPieces()
        }

        function test_create()
        {
            var rook = board.create("Rook", 0, 0)
            verify(rook)
        }

        function test_movesHorizontally()
        {
            var rook = board.create("Rook", 4, 4)

            for(var i = 0; i < 8; ++i)
            {
                if(i !== 4)
                {
                    verify(rook.isValidMove(Qt.point(i, 4)), "Valid move " + Qt.point(i, 4))
                }
            }
        }


        function test_movesVertically()
        {
            var rook = board.create("Rook", 4, 4)

            for(var y = 0; y < 8; ++y)
            {
                if(y !== 4)
                {
                    verify(rook.isValidMove(Qt.point(4, y)), "Valid move " + Qt.point(4, y))
                }
            }
        }
    }
}

