require "rbtree"
class Kdtree
  include Enumerable
  class Node
    def initialize(point, left=nil, right = nil, dim = nil, distance = nil, radius = nil)
      @point = point
      @left = left
      @right = right
      @distance = distance
      @radius = radius
      @dim = dim
    end
    attr_accessor :point, :left, :right, :distance, :radius
  end
  def initialize(points,dim,i=0)
    @points = points
    @root = make_tree(points,dim,i,1)
    @dime = dim
    @best_dist = 1000000000
  end
  def dist(a,b)
    size=a.size
    sum=0
    size.times do |i|
      sum= sum + (a[i]-b[i])*(a[i]-b[i])
    end
    sum
  end
  def quickselect(a, k)
    arr = a.dup
    loop do
      pivot = arr.delete_at(rand(arr.length))
      left, right = arr.partition { |x| x < pivot }
      if k == left.length
        return pivot
      elsif k < left.length
        arr = left
      else
        k = k - left.length - 1
        arr = right
      end
    end
  end
  def make_tree(points,dim,i,index)
 #   print points,"\n"
    root=Kdtree::Node.new(points[0])
    dimension_array=[]
    size = points.size
    points.each do |point|
      dimension_array.push(point[i]) # for each j point we choose dim i
    end
    pivot = quickselect(dimension_array, (dimension_array.size-1) / 2)
 #   puts pivot
    left = []
    right = []
    qwe=0
    points.each do |point|
      left.push(point) if point[i]<pivot
      right.push(point) if point[i]>pivot
    #  puts point[i]
      if point[i]==pivot and qwe==1
    #    puts "asdlksa"
        left.push(point)
        qwe=1
      end
      if point[i]==pivot and qwe==0
    #    puts "asdlksa"
        root=Kdtree::Node.new(point)
        qwe=1
      end

    end
 #   puts root.point[i]
  #  puts "\n\n\n"
    if left.size==0
      root.left=nil
    end
    if left.size==1
      root.left = Kdtree::Node.new(left[0])
    end
    if left.size>1
      root.left = make_tree(left,dim,(i + 1) % dim,index+1)
    end
    if right.size==0
      root.right=nil
    end
    if right.size==1
      root.right=Kdtree::Node.new(right[0])
    end
    if right.size>1
       root.right = make_tree(right,dim,(i + 1) % dim,index+1)
     end

    root
  end
  def near(nearest,size)
    rbtree=MultiRBTree[]
    a=nearest(root,nearest,0,@dime,nil,100000,0,rbtree,size)
  end
  
  def nearest(root,nearest,i,dim,best,best_dist,index,rbtree,size)
    return rbtree if root==nil
    d = dist(root.point,nearest)
    dx = root.point[i]-nearest[i]
    dx2= dx * dx

    root.distance=d
    root.radius = dx2
    rbtree[d]=root.point
    while rbtree.size>size do
      rbtree.pop
    end
    rbtree=nearest(dx > 0 ? root.left : root.right, nearest, (i+1)%dim, dim, best, best_dist,index+1,rbtree,size)
    if (rbtree.size< size || rbtree.last[0]>=dx2) 
      rbtree=nearest(dx > 0 ? root.right : root.left, nearest, (i+1)%dim, dim, best, best_dist,index+1,rbtree,size)
    end
    rbtree
  end
  attr_accessor :root, :point
end

c=Kdtree.new(   [[2, 3],[5, 4], [6, 6], [6, 6],
         [4, 7], [8, 1], [7, 2], [6, 2],[4,6],
         [14, 17],[2, 2],[6, 6],[11, 2],
         [7, 4],[9, 1],[5, 1],[7, 1]],    2)
a=c.near([5, 8],8)
siz=a.size
b=[]
while a.size>0 do
      b.push(a.pop)
end
siz.times do |i|
  print b[siz-1-i][1]," ",b[siz-1-i][0],"\n"
end
a=Time.now()
10000.times do |i|
  c.near([9,2],9)
end
puts Time.now()-a


c=[]
100000.times do |i|
  a=rand(523)
  b=rand(523)
  d=rand(523)
  c.push([a,b,d])
end
asdgh=c
d=Kdtree.new(c,3)
qw=Time.now()
x=d.near([13, 234,444],5)
siz=x.size
b=[]
while x.size>0 do
      b.push(x.pop)
end
ans1=[]
ans2=[]
siz.times do |i|
  print b[siz-1-i][1],b[siz-1-i][0],"\n"
  ans1.push(b[siz-1-i][0])
end

puts t1=Time.now()-qw
def dist(a,b)
    size=a.size
    sum=0
    size.times do |i|
      sum= sum + (a[i]-b[i])*(a[i]-b[i])
    end
    sum
end
dister=[]
qw=Time.now()
c.each do |i|
    dister.push(i.push(dist([13, 234,444],i)))
end
dister=dister.sort{|a,b| a[3] <=> b[3]}
5.times do |i|
  print dister[i],"\n"
  ans2.push(dister[i][3])
end
puts t2=Time.now()-qw
print "Ratio of time taken by brute force and kd tree ", t2/t1,"\n"
print asdgh==c,"\n",ans1==ans2,"\n"
