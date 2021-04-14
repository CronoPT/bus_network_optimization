
import math
import numpy as np

def haversine_distance(a, b, units='m'):
	'''
	| This function computes the haversine distance
	| between point a and point b. The distance is
	| in kilometers.
	'''

	lat1 = a[1]
	lat2 = b[1]
	lon1 = a[0]
	lon2 = b[0]
	lat1, lat2, lon1, lon2 = map(math.radians, [lat1, lat2, lon1, lon2])

	d_lat = lat2 - lat1
	d_lon = lon2 - lon1

	temp = (
		  math.sin(d_lat/2) ** 2
		+ math.cos(lat1)
		* math.cos(lat2)
		* math.sin(d_lon/2) ** 2
	)

	km_distance = 6373.0 * (2 * math.atan2(math.sqrt(temp), math.sqrt(1 - temp)))
	
	if units=='km':
		return km_distance
	elif units=='m':
		return km_distance*1000


def point_belongs_to_line(point, line):
	'''
	| Return true if 'point' belongs to the 'line' and 
	| false otherwise.
	| As we are working with floating point numbers, a
	| small tolerance is allowed.
	'''
	tolerance = 0.00000001

	point  = np.array(point)
	line_point_1 = np.array(line[0])
	line_point_2 = np.array(line[1])
	
	distance_to_1 = np.linalg.norm(point-line_point_1)
	distance_to_2 = np.linalg.norm(point-line_point_2)
	length = np.linalg.norm(line_point_1-line_point_2)

	return abs(distance_to_1+distance_to_2 - length) < tolerance


def closest_point(a, b, p):
	'''                                __
	| This function finds the point in ab 
	| that is closest to point p (outside
	| the line segment).
	'''

	a = np.array(a)
	b = np.array(b)
	p = np.array(p)
	t = - np.inner((b-a), (a-p)) / np.inner((b-a), (b-a))

	if t > 0 and t < 1:
		return ((1-t)*a + t*b).tolist() 

	g0 = np.linalg.norm((a - p))
	g1 = np.linalg.norm((b - p))

	return a.tolist() if g0 < g1 else b.tolist()


class Point: 
	def __init__(self, x, y): 
		self.x = x 
		self.y = y 
  

def on_segment(p, q, r):
	'''
	| Given three colinear points p, q, r, the function checks if
	| point q lies on line segment 'pr' 
	'''
	if ( (q.x <= max(p.x, r.x)) and (q.x >= min(p.x, r.x)) and 
		   (q.y <= max(p.y, r.y)) and (q.y >= min(p.y, r.y))): 
		return True
	return False
  

def orientation(p, q, r):
	'''
	| To find the orientation of an ordered triplet (p,q,r) 
	| function returns the following values: 
	| 0 : Colinear points 
	| 1 : Clockwise points 
	| 2 : Counterclockwise 
	|  
	| See https://www.geeksforgeeks.org/orientation-3-ordered-points/amp/  
	| for details of below formula.  
	'''

	val = (float(q.y - p.y) * (r.x - q.x)) - (float(q.x - p.x) * (r.y - q.y)) 
	if (val > 0): 
		# Clockwise orientation 
		return 1

	elif (val < 0): 
		# Counterclockwise orientation 
		return 2

	else: 
		# Colinear orientation 
		return 0
  

def lines_intersect(line1, line2): 
	'''
	| The main function that returns true if 
	| the line segment 'line1' and 'line2' intersect. 
	'''

	p1 = Point(line1[0][0], line1[0][1])
	q1 = Point(line1[1][0], line1[1][1])
	p2 = Point(line2[0][0], line2[0][1])
	q2 = Point(line2[1][0], line2[1][1])

	# Find the 4 orientations required for  
	# the general and special cases 
	o1 = orientation(p1, q1, p2) 
	o2 = orientation(p1, q1, q2) 
	o3 = orientation(p2, q2, p1) 
	o4 = orientation(p2, q2, q1) 
  
	# General case 
	if ((o1 != o2) and (o3 != o4)): 
		return True
  
	# Special Cases 
  
	# p1 , q1 and p2 are colinear and p2 lies on segment p1q1 
	if ((o1 == 0) and on_segment(p1, p2, q1)): 
		return True
  
	# p1 , q1 and q2 are colinear and q2 lies on segment p1q1 
	if ((o2 == 0) and on_segment(p1, q2, q1)): 
		return True
  
	# p2 , q2 and p1 are colinear and p1 lies on segment p2q2 
	if ((o3 == 0) and on_segment(p2, p1, q2)): 
		return True
  
	# p2 , q2 and q1 are colinear and q1 lies on segment p2q2 
	if ((o4 == 0) and on_segment(p2, q1, q2)): 
		return True
  
	# If none of the cases 
	return False
	