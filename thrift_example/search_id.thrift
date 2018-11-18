namespace cpp Thrift_Example


struct Response{
    1: i32 id,
    2: string name
}

exception InvalidId{
    1:i32 id,
    2:string why
}

service Search{
    Response searchName(1:i32 id)
}