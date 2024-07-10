#!/usr/bin/env python3
import sys,re
g = __import__('ece650-a1-graph')

# YOUR CODE GOES HERE

def pp(x):
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)

class point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __hash__(self):
        """Hashes the x and y coordinate for storage"""
        return hash((self.x, self.y))    

    def __eq__(self, pointB):
        return self.x == pointB.x and self.y == pointB.y

    def __repr__(self):
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'

class line(object):
    """A line between two points"""
    def __init__(self, src, dst, streetName = None):
        self.src = src
        self.dst = dst
        self.streetName = streetName

    def __hash__(self):
        return hash((self.src.x, self.src.y, self.dst.x, self.dst.y))
        
    def __eq__(self, newLine):
        return (self.src.x == newLine.src.x and self.src.y == newLine.src.y and 
                self.dst.x == newLine.dst.x and self.dst.y == newLine.dst.y) or\
                (self.dst.x == newLine.src.x and self.dst.y == newLine.src.y and
                self.src.x == newLine.dst.x and self.src.y == newLine.dst.y)

    def __repr__(self):
        return '['+ str(self.src) + '-->' + str(self.dst) + ']' 

def parseLine(input):
    regexMatch = re.match(r'^\s*(add|rm|mod|gg)\s*((?: +?"[^"]+?" *?))?((?: \(\s*[-]?\d+?\s*,\s*[-]?\d+?\s*\) *?)*)?$', input)
    if(regexMatch and regexMatch != None):
        streetName = None
        points = None
        action = regexMatch.group(1).strip()
        if(action == 'add' or action == 'mod'):
            if(not regexMatch.group(2) or not regexMatch.group(3) or len(regexMatch.group(2)) == 0 or len(regexMatch.group(3)) == 0):
                raise Exception('street name or coordinates are missing')
            streetName = regexMatch.group(2).replace('"', '').lower().strip()
            points = re.findall(r'\(\s*[-]?\d+\s*,\s*[-]?\d+\s*\)', regexMatch.group(3))
            points = [point.replace(" ", "") for point in points]  # removing spaces
            if(len(points) < 2):
                raise Exception('Not enough points to form a street')
            return action, streetName, points
        elif(action == 'gg'):
            if(regexMatch.group(2) != None and len(regexMatch.group(2)) != None):
                raise Exception('invalid input for gg action.')
            return action, streetName, points
        elif(action == 'rm'):
            if(not regexMatch.group(2) or len(regexMatch.group(2)) == 0):
                raise Exception('street name is missing.')
            if(regexMatch.group(3) != None and len(regexMatch.group(3)) != 0):
                raise Exception('invalid input for rm action.')
            streetName = regexMatch.group(2).replace('"', '').lower().strip()
            return action, streetName, points
    else:
        raise Exception("Invalid input. Sample input -> add \"weber\" (2,3) (3,5)")        

def main():
    graph = g.Graph()
    while True:
        try:
            input = sys.stdin.readline()
            if input == "":
                    break
            try:
                action, streetName, points = parseLine(input)
                if(action == 'add'):
                    graph.addStreet(streetName, points)
                elif(action == 'rm'):
                    graph.removeStreet(streetName)    
                elif(action == 'gg'):
                    graph.gg()
                elif(action == 'mod'):
                    graph.modifyStreet(streetName, points)
                else:
                    raise Exception('Invalid input/action')
            except Exception as exception:
                print('Error: ' + str(exception), file=sys.stderr)
        except KeyboardInterrupt:
            sys.exit(0)        
    sys.exit(0)

if __name__ == "__main__":
    main()