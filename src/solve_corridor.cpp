#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
    typedef std::unique_ptr<Position> PositionPtr;

public:
    // constructor from coordinates
    Position(int _x, int _y) : Point(_x, _y) {}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    // constructor with
    Position(int _x, int _y, int distance): Point(_x,_y) {distance_=distance;}

    int distToParent()
    {
        return distance_;
    }

    std::vector<PositionPtr> children()
    {
        // this method should return  all positions reachable from this one
        std::vector<PositionPtr> generated;

        // TODO add free reachable positions from this point


        std::vector<std::pair<int,int>> voisins = {{-1,0},{1,0},{0,-1},{0,1}};

        // On vérifie les 4 voisins de la cellule
        // S'il s'agit de cases vides, on les ajoute au vecteur generated
        for(const auto& i : voisins ){
            if(maze.isFree(x+i.first,y+i.second) == 1)
                generated.push_back(PositionPtr(new Position (x+i.first,y+i.second)));
        


    }


        return generated;
    }

    bool is_corridor(int x,int y)
    {

        //on est dans un corridor si on est dans un couloir, ou dans un coin
        
        //couloir horizontal et vertical (1=trou)
        if((maze.isFree(x+1,y)==1 && maze.isFree(x-1,y)==1 )&& (maze.isFree(x,y+1)==0&& maze.isFree(x,y-1)==0))
            return true;
        else if((maze.isFree(x+1,y)==0 && maze.isFree(x-1,y)==0 )&& (maze.isFree(x,y+1)==1&& maze.isFree(x,y-1)==1))
            return true;
            
        //coins
        for(int i=-1;i<=1;i+=2)
        {
            for(int j=-1;j<=1;j+=2)
            {
                if((maze.isFree(x+i,y)==0 &&maze.isFree(x,y+j)==0 )&& (maze.isFree(x-i,y)==1&& maze.isFree(x,y-j)==1))
                return true;
            }
        }
        
        return false;
    }
    
protected:
    int distance_;
};



int main( int argc, char **argv )
{
    // load file
    std::string filename = "../mazes/maze.png";
    if(argc == 2)
        filename = std::string(argv[1]);

    // let Point know about this maze
    Position::maze = ecn::Maze(filename);

    // initial and goal positions as Position's
    Position start = Position::maze.start(),
             goal = Position::maze.end();

    // call A* algorithm
    ecn::Astar(start, goal);

    // save final image
    Position::maze.saveSolution("cell");
    cv::waitKey(0);

}

