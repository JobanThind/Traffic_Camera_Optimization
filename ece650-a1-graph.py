# took reference from geeks for geeks and stack overflow to compute mathematical things 

import math,sys
a1 = __import__('ece650-a1')

class Graph():
    def gg(self):
        self.resetGraph()
        self.getVertices()
        self.getEdges()
        self.generate_input_for_a2()

    # def generateInputForA2(self):
    #     n = len(self.edges)
    #     print("V", len(self.final_vertices))
    #     sys.stdout.flush()
    #     print('E {', end="", file = sys.stdout)
    #     self.edges = sorted(self.edges, key=lambda edge:[edge.src.x, edge.src.y, edge.dst.x, edge.dst.y])
        
    #     for index, edge in enumerate(self.edges):
    #         strToPrint = '<'+ str(self.final_vertices.index(edge.src)+1)+ ','+ str(self.final_vertices.index(edge.dst)+1)+ '>'

    #         if(index != n-1):
    #             strToPrint = strToPrint + ','
    #         print(strToPrint, end="", file = sys.stdout)
    #     print('}', file = sys.stdout)
    #     print("end of E")
    #     sys.stdout.flush()

    def generate_input_for_a2(self):
        n = len(self.edges)
        print(f"V {len(self.final_vertices)}")
        print('E {', end="", file=sys.stdout)
        sys.stdout.flush()
        self.edges.sort(key=lambda edge: (edge.src.x, edge.src.y, edge.dst.x, edge.dst.y))

        for index, edge in enumerate(self.edges):
            src_index = self.final_vertices.index(edge.src) + 1
            dst_index = self.final_vertices.index(edge.dst) + 1
            str_to_print = f'<{src_index},{dst_index}>'

            if index != n - 1:
                str_to_print += ','

            print(str_to_print, end="", file=sys.stdout)

        print('}', file=sys.stdout)
        print("end of E")
        sys.stdout.flush()    
        
    def modifyStreet(self, streetName, points):
        if(not self.streetExists(streetName)):
            raise Exception('Street does not exist')
        self.removeStreet(streetName)
        self.addStreet(streetName, points)

    def removeStreet(self, streetName):
        if(not self.streetExists(streetName)):
            raise Exception('Street does not exist')
        self.lineSegments = [lineSegment for lineSegment in self.lineSegments if(lineSegment.streetName != streetName)]
        self.resetGraph()
        self.streetMap.pop(streetName)

    def __init__(self):
        self.streetMap = dict()
        self.intersectionPoints = set()
        self.final_vertices = []
        self.edges = []
        self.lineSegments = []

    def addStreet(self, streetName, points):
        if(self.streetExists(streetName)):
            raise Exception('Street already exists')
        parsedPoints = []
        self.resetGraph()
        for point in points:
            [x,y] = point.replace("'", "").replace('(','').replace(')','').split(',')
            parsedPoints.append(a1.point(int(x), int(y)))
        self.streetMap[streetName] = parsedPoints
        for i in range(0, len(parsedPoints) - 1):
            l = a1.line(parsedPoints[i], parsedPoints[i+1], streetName)
            self.lineSegments.append(l) 

    def has_intersection(self, line1, line2):
        orientation_group_three = self.checkOrientation(line2.src, line2.dst, line1.src)
        orientation_group_four = self.checkOrientation(line2.src, line2.dst, line1.dst)
        orientation_group_one = self.checkOrientation(line1.src, line1.dst, line2.src)
        orientation_group_two = self.checkOrientation(line1.src, line1.dst, line2.dst)
        
        if orientation_group_one == "col" and self.liesOnLineSeg(line1.src, line2.src, line1.dst):
            return True, line2.src
        elif orientation_group_two == "col" and self.liesOnLineSeg(line1.src, line2.dst, line1.dst):
            return True, line2.dst
        elif orientation_group_three == "col" and self.liesOnLineSeg(line2.src, line1.src, line2.dst):
            return True, line1.src
        elif orientation_group_four == "col" and self.liesOnLineSeg(line2.src, line1.dst, line2.dst):
            return True, line1.dst
        else:
            return (orientation_group_one != orientation_group_two) and (orientation_group_three != orientation_group_four), None

    def liesOnEdge(self, p1, p2, p3):
        a = p3.y - p1.y
        b = p2.x - p1.x
        c = p2.y - p1.y
        d = p3.x - p1.x
        crossProduct = (a) * (b) - (d) * (c)
        dotProduct = (d)*(b) + (a)*(c)
        e = 1e-12
        if abs(crossProduct) > e:
            return False
        if dotProduct < 0:
            return False
        baSq = (b)**2 + (c)**2
        if dotProduct > baSq:
            return False
        return True
            
    def streetExists(self,street):
        return (street in self.streetMap.keys())
    
    def resetGraph(self):
        self.edges = []
        self.final_vertices = []
        self.intersectionPoints = set()

    def liesOnLineSeg(self, src, intersection, dst):
        return ( (intersection.x <= max(src.x, dst.x)) and (intersection.x >= min(src.x, dst.x)) and 
           (intersection.y <= max(src.y, dst.y)) and (intersection.y >= min(src.y, dst.y)))

    def getEdges(self):
        add, remove = [], []
        for edge in self.edges:
            intPointVsDistMap = dict()
            intersectionsToCheck = []
            for intersection in self.intersectionPoints:
                if(edge.dst != intersection and intersection != edge.src and self.liesOnEdge(edge.src, edge.dst, intersection)):
                    intersectionsToCheck.append(intersection)
            if(len(intersectionsToCheck) == 0):
                pass
            elif(len(intersectionsToCheck) == 1):
                lineToAdd = a1.line(intersectionsToCheck[0], edge.dst)

                if(lineToAdd not in add):
                    add.append(a1.line(intersectionsToCheck[0], edge.dst))
                if(edge not in remove):
                    remove.append(edge)                    
            else:
                temp = edge.dst
                for intersection in intersectionsToCheck:
                    intPointVsDistMap[intersection] = math.sqrt((edge.dst.x - intersection.x)**2 + (edge.dst.y - intersection.y)**2)
                intPointVsDistMap = sorted(intPointVsDistMap.items(), key=lambda x:x[1])
                for intPoint, dist in intPointVsDistMap:
                    lineToAdd = a1.line(temp, intPoint) 
                    if(lineToAdd not in add):
                        add.append(lineToAdd)
                    temp = intPoint
                    if(edge not in remove):
                        remove.append(edge)   
        for edgeToAdd in add:
            if(edgeToAdd not in self.edges):
                self.edges.append(edgeToAdd)
        for edgeToRemove in remove:
            self.edges.remove(edgeToRemove)

    def checkOrientation(self, p1, p2, p3):
        yx = (float(p2.y - p1.y) * (p3.x - p2.x))
        xy = (float(p2.x - p1.x) * (p3.y - p2.y))
        if(yx - xy < 0):
            return "cl"
        elif(yx - xy > 0):
            return "countercl"
        else:
            return "col"
    
    def reverseWords(self, s: str) -> str:
        res = []
        temp = ""
        for c in s:
            if c != " ":
                temp += c 
            elif temp != "":
                res.append(temp)
                temp = ""
        if temp != "":
            res.append(temp)
        return " ".join(res[::-1])

    def get_intersections(self, line1, line2):
        try:
            xcoor, ycoor = None, None
            
            x3, y3 = line2.src.x, line2.src.y
            x4, y4 = line2.dst.x, line2.dst.y
            x1, y1 = line1.src.x, line1.src.y
            x2, y2 = line1.dst.x, line1.dst.y
            
            do_intersect, collinear_point = self.has_intersection(line1, line2)
            
            if not do_intersect:
                return
            
            xnum = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4))
            xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
            ynum = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4))
            yden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
            
            if xden != 0:
                xcoor = xnum / xden
                ycoor = ynum / yden
            else:
                xcoor = collinear_point.x
                ycoor = collinear_point.y
                
            intersection_point = a1.point(xcoor, ycoor)
            self.intersectionPoints.add(intersection_point)
            
            points_to_add = [line1.src, line1.dst, line2.src, line2.dst, intersection_point]
            for point in points_to_add:
                if point not in self.final_vertices:
                    self.final_vertices.append(point)
                    
            line_segments_to_add = [a1.line(line1.src, intersection_point), 
                                    a1.line(line1.dst, intersection_point), 
                                    a1.line(line2.src, intersection_point), 
                                    a1.line(line2.dst, intersection_point)]
                                    
            for line_segment in line_segments_to_add:
                if line_segment not in self.edges and line_segment.src != line_segment.dst:
                    self.edges.append(line_segment)
        except Exception as e:
            raise e

    def getVertices(self):
        for i in range(0, len(self.lineSegments)-1):
            for j in range(i+1, len(self.lineSegments)):
                if(self.lineSegments[i].streetName != self.lineSegments[j].streetName):
                    self.get_intersections(self.lineSegments[i], self.lineSegments[j])

    # def print_edges_and_vertices(self):
    #     # Print vertices
    #     print('V = {', file=sys.stdout)
    #     self.final_vertices = sorted(self.final_vertices, key=lambda vertex: (vertex.x, vertex.y))
    #     for i, vertex in enumerate(self.final_vertices):
    #         vertex_str = str(vertex).replace(")", "").replace("(", "").replace(" ", "")
    #         print(f'  {i + 1}: ({vertex_str})', file=sys.stdout)
    #     print('}', file=sys.stdout)
        
    #     # Print edges
    #     print('E = {', file=sys.stdout)
    #     self.edges = sorted(self.edges, key=lambda edge: (edge.src.x, edge.src.y, edge.dst.x, edge.dst.y))
    #     for index, edge in enumerate(self.edges):
    #         src_index = self.final_vertices.index(edge.src) + 1
    #         dst_index = self.final_vertices.index(edge.dst) + 1
    #         edge_str = f'  <{src_index},{dst_index}>'
    #         if index != len(self.edges) - 1:
    #             edge_str += ','
    #         print(edge_str, file=sys.stdout)
    #     print('}', file=sys.stdout)    