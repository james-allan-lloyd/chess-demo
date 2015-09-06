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

        function test_queenMovesHorizontally()
        {
            var queen = board.create("Queen", 4, 4)

            for(var i = 0; i < 8; ++i)
            {
                if(i !== 4)
                {
                    verify(queen.isValidMove(Qt.point(i, 4)), "Valid move " + Qt.point(i, 4))
                }
            }
        }
    }
}

