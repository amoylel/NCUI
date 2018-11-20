;(function(){
    $(function(){
       
//     $('div, body, html').addClass('drag');
//     $('button, input, a, textarea').removeClass('drag');
//     $('button, input, a, textarea').addClass('no-drag');
//     $('iframe, .container').removeClass('drag');
       include('BrowserWindow');
       var win = BrowserWindow.currentWindow;
        win.on('maximize', function() {
            $('.restorebtn').removeClass('hidden');
            $('.maxbtn').addClass('hidden');
        });

        win.on('restore', function() {
            $('.maxbtn').removeClass('hidden');
            $('.restorebtn').addClass('hidden');
        });

        $('.minbtn').click(function() {
            win.minimize();
        });

        $('.maxbtn').click(function() {
            win.maximize();
        });

        $('.restorebtn').click(function() {
            win.restore();
        });

        $('.closebtn').click(function() {
            win.close();
        })
    });
})();
