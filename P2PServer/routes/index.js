var express = require('express');
var router = express.Router();

var allowedIds = ['nycshisan'];
var ipPairs = [];

var normalizeIP = function(ip) {
  var normalizedIP = ip;

  if (ip.startsWith('::ffff:')) {
    normalizedIP = ip.slice(7);
  }

  return normalizedIP;
};

router.post('/', function(req, res, next) {
  var id = req.body.id;
  var ip = req.ip;

  console.log(req.body);

  if (allowedIds.indexOf(id) === -1) {
    res.send('Not allowed!');
  } else {
    ipPairs[id] = normalizeIP(ip);
    res.send('OK!');
  }
});

router.get('/:id', function(req, res, next) {
  var id = req.params.id;

  if (allowedIds.indexOf(id) === -1) {
    res.send('Not allowed!');
  } else {
    res.send(ipPairs[id]);
  }
});

module.exports = router;
