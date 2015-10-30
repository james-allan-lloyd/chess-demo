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
            var knight = board.create("knight", 0, 0)
            verify(knight)
        }

        function test_knightMoves()
        {
            var knight = board.create("knight", 4, 4)
            verify(knight.isValidMove(Qt.point(3, 2))); // up left
            verify(knight.isValidMove(Qt.point(5, 2))); // up right
            verify(knight.isValidMove(Qt.point(3, 6))); // down left
            verify(knight.isValidMove(Qt.point(5, 6))); // down right

            verify(knight.isValidMove(Qt.point(2, 3))); // left up
            verify(knight.isValidMove(Qt.point(2, 5))); // left down
            verify(knight.isValidMove(Qt.point(6, 3))); // right up
            verify(knight.isValidMove(Qt.point(6, 5))); // right down
        }

        function test_knightDoesNotAttackAllies()
        {
            var knight = board.create("knight", 4, 4)
            board.create("pawn", 3, 2)
            verify(!knight.isValidMove(Qt.point(3, 2)));
        }


        function test_knightDoesTakeEnemies()
        {
            var knight = board.create("knight", 4, 4, Chess.BoardModel.BLACK)
            board.create("pawn", 3, 2, Chess.BoardModel.WHITE)
            verify(knight.isValidMove(Qt.point(3, 2)));
        }
    }

}

