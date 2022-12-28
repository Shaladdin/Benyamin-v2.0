const express = require('express')
const router = express.Router()

// middleware that is specific to this router
// router.use((req, res, next) => {
//     console.log('Time: ', Date.now(), 'request: ', req);
//     next();
// })

router.get('/ping', (req, res) => {
    res.json({ ping: 'pong' });
})
module.exports = router