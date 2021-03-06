# if else test
# --output:start
# test1 test2 test3
# [1, 3, 8, 4, 5, 2]
# [1, 2, 3, 4, 5, 8]
# [8, 5, 4, 3, 2, 1]
# [1, 2, 3, 4, 5, 8]
# [16, 10, 8, 6, 4, 2]
# [8, 5, 4, 3, 2, 1]
# [8, 4, 2]
# 2
# false
# 4
# [8, 2]
# [8, 5, 4, 2, 1]
# []
# [8, 5, 4, 2, 1, 70, 78, 84]
# [8, 5, 45, 2, 15, 70, 78, 84]
# [8, 5, 2, 70, 78, 84]
# false
# false
# true
# false
# [1, 2, 3]
# [5, 2, 70]
# before del: [8, 5, 2, 70, 78, 84]
# after del: [8, 78, 84]
# 8
# 78
# 84
# --output:end

str_arr = ["test1", "test2", "test3"]
echo $@{str_arr}

arr = [1]
arr.append(3, 4, 2)
arr.insert(2, 8)
arr.insert(4, 5)
print(arr)
arr.sort()
print(arr)
arr.sort(func(a, b) {return a > b})
print(arr)
arr.sort(func(a, b) {return a < b})
print(arr)
arr.map(func(i) {return i*2})
arr.reverse()
print(arr)

arr.map(func(e) {
  return e/2
})

print(arr)

q = []
arr.for_each(func(e) {
  if e % 2 == 0 {
    q.append(e)
  }
})

print(q)
q.append(4)
print(q.count(4))
print(arr.index(6))
print(arr.index(2))
q.remove(4)
print(q)
arr.pop(3)
print(arr)
q.clear()
print(q)
arr.extend([70,78,84])
print(arr)
arr[2] = 45
arr[4] = 15
print(arr)
del arr[4]
del arr[2]
print(arr)
print([1,3] == 4)
print([1,3] == [2,4, 8])
print([1,3] == [1,3])
print([1,3] == [2,4])
p = type([])([1, 2, 3])
print(p)
arr2 = arr[1:4]
print(arr2)
print("before del: ", arr)
del arr[1:4]
print("after del: ", arr)

for i in arr {
  print(i)
}
