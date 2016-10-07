
type inttree = Empty | Node of int * inttree * inttree

(* use this function in fromList *)
let rec insert t i =
  match t with
      Empty -> Node(i,Empty,Empty)
    | Node(j,l,r) -> 
      if i=j 
      then t 
      else if i < j 
      then Node(j,insert l i,r)
      else Node(j,l,insert r i)

(* no need for this function; it is just an example *)
let rec member t i =
  match t with
      Empty -> false
    | Node(j,l,r) -> i=j || (i < j && member l i) || member r i

(* put fromList, sum1, prod1, avg1, map, and negateAll here *)

(* make a sorted tree containing exactly the ints in the list without repeats *)
let fromList a =
  List.fold_left insert Empty a

(* compute the sum, sum of an empty tree is 0 *)
let rec sum1 t =
  match t with
    Empty -> 0
  | Node(i,Empty,Empty) -> i
  | Node(i,l,Empty) -> i + sum1 l
  | Node(i,Empty,r) -> i + sum1 r
  | Node(i,l,r) -> i + sum1 l + sum1 r

(* compute the product, procuct of an empty tree is 1 *)
let rec prod1 t =
  match t with
    Empty -> 1
  | Node(i,Empty,Empty) -> i
  | Node(i,l,Empty) -> i * prod1 l
  | Node(i,Empty,r) -> i * prod1 r
  | Node(i,l,r) -> i * prod1 l * prod1 r


let helpDiv (sum, count) = sum / count
let sumTuple ((i1, n1), (i2, n2)) = (i1 + i2, n1 + n2)
let rec helpAvg sum n t =
  match t with
  | Empty -> (sum, n)
  | Node(i,Empty,Empty) -> (sum + i, n + 1)
  | Node(i,l,Empty) -> sumTuple ( (i, 1),  helpAvg sum n l )
  | Node(i,Empty,r) -> sumTuple ( (i, 1),  helpAvg sum n r )
(*   | Node(i,l,r) -> i * prod1 l * prod1 r *)

(* compute the average *)
let avg1 t =
  match t with
    Empty -> raise Division_by_zero
  | Node(i,l,r) ->  helpDiv (helpAvg i 1 r)
    
  

let rec fold f a t =
  match t with
      Empty -> a
    | Node(j,l,r) -> fold f (fold f (f a j) l) r

(* put sum2, prod2, and avg2 here *)

type 'a iterator = Nomore | More of 'a * (unit -> 'a iterator)

let rec iter t =
  let rec f t k =
    match t with
	Empty -> k ()
      | Node(j,l,r) -> More(j, fun () -> f l (fun () -> f r k))
  in f t (fun () -> Nomore)

(* challenge problem: put optionToException and exceptionToOption here *)

(* a little testing -- commented out since the functions do not exist yet *)


let tr = fromList [0;1;2;3;4;5;6;7;8;9;9;9;1] (* repeats get removed *)
let print_ans f t = print_string (string_of_int (f t)); print_string "\n"
(*
let _ = print_ans sum1 tr
let _ = print_ans prod1 tr
let _ = print_ans avg1 tr
let _ = print_ans sum2 tr
let _ = print_ans prod2 tr
let _ = print_ans avg2 tr
let _ = print_ans sum3 tr
let _ = print_ans prod3 tr
let _ = print_ans avg3 tr
*)
