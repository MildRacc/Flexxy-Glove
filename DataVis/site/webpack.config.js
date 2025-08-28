const path = require("path");

module.exports = {
  entry: "./main.js", // Entry point
  output: {
    filename: "bundle.js", // Output file name
    path: path.resolve(__dirname, "dist"), // Output directory
  },
  mode: "development", // Mode: 'development' or 'production'
};
