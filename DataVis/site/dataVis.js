import * as d3 from 'd3';
import { add } from 'three/tsl';

const data1 = [/* array data */];
const data2 = [/* array data */];
const data3 = [/* array data */];
const data4 = [/* array data */];
const data5 = [/* array data */];

const svg = d3.select("body").append("svg")
    .attr("width", 960)
    .attr("height", 500);

const x = d3.scaleLinear()
    .domain([0, d3.max(data1, (d, i) => i)])
    .range([0, 960]);

const y = d3.scaleLinear()
    .domain([0, d3.max([d3.max(data1), d3.max(data2), d3.max(data3), d3.max(data4), d3.max(data5)])])
    .range([500, 0]);

const line = d3.line()
    .x((d, i) => x(i))
    .y(d => y(d));

svg.append("path")
    .datum(data1)
    .attr("fill", "none")
    .attr("stroke", "steelblue")
    .attr("stroke-width", 1.5)
    .attr("d", line);

svg.append("path")
    .datum(data2)
    .attr("fill", "none")
    .attr("stroke", "red")
    .attr("stroke-width", 1.5)
    .attr("d", line);

svg.append("path")
    .datum(data3)
    .attr("fill", "none")
    .attr("stroke", "green")
    .attr("stroke-width", 1.5)
    .attr("d", line);

svg.append("path")
    .datum(data4)
    .attr("fill", "none")
    .attr("stroke", "orange")
    .attr("stroke-width", 1.5)
    .attr("d", line);

svg.append("path")
    .datum(data5)
    .attr("fill", "none")
    .attr("stroke", "purple")
    .attr("stroke-width", 1.5)
    .attr("d", line);

export function addData(arr, data) {

    if(typeof arr !== 'int' || typeof data !== 'int') {
        return 'Invalid data type';
    }

    switch(arr) {
        case 0:
            data1.shift();
            data1.push(data);
            break;
        case 1:
            data2.shift();
            data2.push(data);
            break;
        case 2:
            data3.shift();
            data3.push(data);
            break;
        case 3:
            data4.shift();
            data4.push(data);
            break;
        case 4:
            data5.shift();
            data5.push(data);
            break;
        default:
            return 'Out of bounds array';
    }

}