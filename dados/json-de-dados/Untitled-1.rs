// input
{
   autentication: {
        autorization: true
   },
   sensors: {
        temperature: {
            type: "LSM6DS3",
            output: 25,
            pin: "I2C"
        },
        giroscopy: {
            type: "LSM6DS3",
            output: 0,
            pin: "I2C"
        },
        speedometer: {
            type: "speed",
            output: 3,
            pin: "GPI5"
        },
        battery: {
            type: "battery",
            output: 70,
            pin: "GPI6"
        }
   }
}

{autentication: {autorization: true},sensors: {temperature: {type: "LSM6DS3",output: 25,pin: "I2C"},giroscopy: {type: "LSM6DS3",output: 0,pin: "I2C"},speedometer: {type: "speed",output: 3,pin: "GPI5"},battery: {type: "battery",output: 70,pin: "GPI6"}}}

{
    \"autentication\":{
        \"autorization\":true
    },
    \"sensors\":{
        \"temperature\":{
            \"type\":\"LSM6DS3\",
            \"output\":25,
            \"pin\":\"I2C
        \"},
        \"gyroscopy\":{
            \"type\":\"LSM6DS3\",
            \"output\":0,
            \"pin\":\"I2C
        \"},
        \"speedometer\":{
            \"type\":\"speed\",
            \"output\":3,
            \"pin\":\"GPI5
        \"},
        \"battery\":{
            \"type\":\"battery\",
            \"output\":70,
            \"pin\":\"GPI6
        \"}
    }
}



