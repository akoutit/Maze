#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
    typedef std::unique_ptr<Position> PositionPtr;

protected:
    int distance_; // Distance d'un point Ã  un autre (en ligne droite)

public:
    // constructor from coordinates
    Position(int _x, int _y) : Point(_x, _y) {}

    // constructor with distance
    Position(int _x, int _y, int distance): Point(_x,_y) {distance_=distance;}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    int distToParent()
    {
        // in line-based motion, the distance to the parent is at least 1
        return distance_;
    }

    bool is_corridor(int x,int y) // Permet de vérifier si on est toujours dans le mÃªme corridor
    {
        // Si les deux cases à  droite et à  gauche sont vides et que les cases haut et bas sont des murs
        // ou inversement, alors on est toujours dans le mÃªme corridor
        if((maze.isFree(x+1,y)==1 && maze.isFree(x-1,y)==1) && (maze.isFree(x,y+1)==0 && maze.isFree(x,y-1)==0))
            return true;
        else if((maze.isFree(x+1,y)==0 && maze.isFree(x-1,y)==0) && (maze.isFree(x,y+1)==1 && maze.isFree(x,y-1)==1))
            return true;
        else
            return false;
    }

    std::vector<PositionPtr> children()
    {
        // this method should return  all positions reachable from this one
        std::vector<PositionPtr> generated;

        //Initialisation de la distance au parent
        int dist=1;

        std::vector<std::pair<int,int>> voisins = {{-1,0},{1,0},{0,-1},{0,1}};

        // On vÃ©rifie les lignes Ã  gauche, Ã  droite, en bas et en haut du parent
        // S'il s'agit de lignes vides, on les ajoute au vecteur generated
        for(const auto& i : voisins ){
            dist=1;

            if(maze.isFree(x+i.first*dist,y+i.second*dist) == 1)
            {
                // On avance en ligne droite tant que la cellule suivante est vide
                while(is_corridor(x+i.first*dist,y+i.second*dist))
                    dist++;
                generated.push_back(PositionPtr(new Position(x+i.first*dist,y+i.second*dist,dist)));
            }
        }

        return generated;
    }
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

