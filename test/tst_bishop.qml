import QtQuick 2.3
import QtQuick.Window 2.2
import QtTest 1.0
import com.vividcode.Chess 1.0 as Chess

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
            var bishop = board.create("bishop", 0, 0)
            verify(bishop)
        }


        function checkDirection(piece, xdir, ydir)
        {
            var p = piece.position;
            p = Qt.point(p.x + xdir, p.y + ydir)
            while(p.x >= 0 && p.y >= 0 && p.x <= 7 && p.y <= 7)
            {
                verify(piece.isValidMove(p), "Valid move " + p);
                p = Qt.point(p.x + xdir, p.y + ydir)
            }
        }


        function test_queenMovesDiagonally()
        {
            var bishop = board.create("bishop", 4, 4)

            checkDirection(bishop, -1, -1) // left, top
            checkDirection(bishop, -1, 1)  // left, bottom
            checkDirection(bishop, 1, -1)  // right, top
            checkDirection(bishop, 1, 1)   // right, bottom
        }
    }

}

