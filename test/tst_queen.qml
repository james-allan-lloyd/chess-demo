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


        function test_queenMovesVertically()
        {
            var queen = board.create("Queen", 4, 4)

            for(var y = 0; y < 8; ++y)
            {
                if(y !== 4)
                {
                    verify(queen.isValidMove(Qt.point(4, y)), "Valid move " + Qt.point(4, y))
                }
            }
        }


        function checkDirection(queen, xdir, ydir)
        {
            var p = queen.currentPosition;
            p = Qt.point(p.x + xdir, p.y + ydir)
            while(p.x >= 0 && p.y >= 0 && p.x <= 7 && p.y <= 7)
            {
                verify(queen.isValidMove(p), "Valid move " + p);
                p = Qt.point(p.x + xdir, p.y + ydir)
            }
        }


        function test_queenMovesDiagonally()
        {
            var queen = board.create("Queen", 7, 4)

            checkDirection(queen, -1, -1) // left, top
            checkDirection(queen, -1, 1)  // left, bottom
            checkDirection(queen, 1, -1)  // right, top
            checkDirection(queen, 1, 1)   // right, bottom

            // verify(queen.isValidMove(Qt.point(4, 4)));
            // verify(queen.moveTo(Qt.point(4, 4)));

            // checkDirection(queen, -1, -1) // left, top
            // checkDirection(queen, -1, 1)  // left, bottom
            // checkDirection(queen, 1, -1)  // right, top
            // checkDirection(queen, 1, 1)   // right, bottom

            //var p = queen.currentPosition;

            // var xdir = -1
            // var ydir = -1
            // while(p.x > 0 && p.y > 0)
            // {
            //     p = Qt.point(p.x + xdir, p.y + ydir)
            //     verify(queen.isValidMove(p), "Valid move " + p);
            // }

            //xdir = -1
            //ydir = 1
            //while(p.x > 0 && p.y > 0 && p.x < 7 && p.y < 7)
            //{
            //    p = Qt.point(p.x + xdir, p.y + ydir)
            //    verify(queen.isValidMove(p), "Valid move " + p);
            //}
        }
    }
}

